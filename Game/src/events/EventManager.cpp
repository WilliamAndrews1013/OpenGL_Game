#include <assert.h>
#include "events/EventManager.hh"
#include <algorithm>

namespace Events {

  EventManager gEventManager;
  
  void EventManager::Shutdown()
  {
    m_Subscribers.clear();
  }

  void EventManager::Subscribe(EventType eventType, std::unique_ptr<IEventHandlerWrapper>&& handler, EventID eventId)
  {
    if (eventId) {
      auto subscribers = m_Subscribers.find(eventId);
      if (subscribers != m_Subscribers.end()) {
        auto& handlersMap = subscribers->second;
        handlersMap.emplace_back(std::move(handler));
        return;
      }
      m_SubscribersByEventId[eventType][eventId].emplace_back(std::move(handler));
    } else {
      auto subscribers = m_Subscribers.find(eventType);
      if (subscribers != m_Subscribers.end()) {
        auto& handlers = subscribers->second;
        for (auto& it : handlers) {
          if (it->GetType() == handler->GetType()) {
            assert(false && "Handler already subscribed to this event type");
            return;
          }
        }
        handlers.emplace_back(std::move(handler));
      } else {
        m_Subscribers[eventType].emplace_back(std::move(handler));
      }
    }
  }

  void EventManager::Unsubscribe(EventType eventType, const std::string& handlerName, EventID eventID)
  {
    if (eventID) {
      auto subscribers = m_SubscribersByEventId.find(eventType);
      if (subscribers != m_SubscribersByEventId.end()) {
        auto& handlersMap = subscribers->second;
        auto handlers = handlersMap.find(eventID);
        if (handlers != handlersMap.end()) {
          auto& callbacks = handlers->second;
          for (auto it = callbacks.begin(); it != callbacks.end(); ++it) {
            if ((*it)->GetType() == handlerName) {
              callbacks.erase(it);
              return;
            }
          }
        }
      }
    } else {
      auto subscribers = m_Subscribers.find(eventType);
      if (subscribers != m_Subscribers.end()) {
        auto& handlers = subscribers->second;
        for (auto it = handlers.begin(); it != handlers.end(); ++it) {
          if ((*it)->GetType() == handlerName) {
            handlers.erase(it);
            return;
          }
        }
      }
    }
  }

  void EventManager::TriggerEvent(const Event& event, EventID eventId)
  {
    for (auto& handler : m_Subscribers[event.GetEventType()]) {
      handler->Exec(event);
    }

    auto& handlersMap = m_SubscribersByEventId[event.GetEventType()];
    auto handlers = handlersMap.find(eventId);
    if (handlers != handlersMap.end()) {
      auto& callbacks = handlers->second;
      for (auto it = callbacks.begin(); it != callbacks.end();) {
        auto& handler = *it;
        handler->Exec(event);
        if (handler->IsDestroyOnSuccess()) {
          it = callbacks.erase(it);
        } else {
          ++it;
        }
      }
    }
  }

  void EventManager::QueueEvent(std::unique_ptr<Event>&& event, EventID eventId)
  {
    m_EventsQueue.emplace_back(std::move(event), eventId);
  }

  void EventManager::DispatchEvents()
  {
    for (auto eventIt = m_EventsQueue.begin(); eventIt != m_EventsQueue.end();) {
      if (!eventIt->first->m_IsHandled) {
        TriggerEvent(*eventIt->first.get(), eventIt->second);
        eventIt = m_EventsQueue.erase(eventIt);
      } else {
        ++eventIt;
      }
    }
  }
}
