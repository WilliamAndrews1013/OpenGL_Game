#pragma once
#include "EventHandler.hh"
#include <vector>
#include <unordered_map>
#include <memory>
#include <cstdint>

namespace Events {
  using EventType = std::uint32_t;
  using EventID = std::uint64_t;

  class EventManager {
  public:
    EventManager() = default;
    EventManager(const EventManager&) = delete;
    const EventManager* operator=(const EventManager&) = delete;

    void Shutdown();

    void Subscribe(EventType eventType, std::unique_ptr<IEventHandlerWrapper>&& handler, EventID eventID);
    void Unsubscribe(EventType eventType , const std::string& handlerName, EventID eventID);
    void TriggerEvent(const Event& event, EventID eventId);
    void QueueEvent(std::unique_ptr<Event>&& event, EventID eventId);
    void DispatchEvents();

  private:
    std::vector<std::pair<std::unique_ptr<Event>, EventID>> m_EventsQueue;
    std::unordered_map<EventType, std::vector<std::unique_ptr<IEventHandlerWrapper>>> m_Subscribers;
    std::unordered_map<EventType, std::unordered_map<EventID, std::vector<std::unique_ptr<IEventHandlerWrapper>>>> m_SubscribersByEventId;

  };

  extern EventManager gEventManager;

  template<typename EventType>
  inline void Subscribe(const EventHandler<EventType>& callback, EventID eventId = 0, const bool unsubscribeOnSuccess = false)
  {
    std::unique_ptr<IEventHandlerWrapper> handler = std::make_unique<EventHandlerWrapper<EventType>>(callback, unsubscribeOnSuccess);
    gEventManager.Subscribe(EventType::GetStaticEventType(), std::move(handler), eventId);
  }

  template<typename EventType>
  inline void Unsubscribe(const EventHandler<EventType>& callback, EventID eventId = 0)
  {
    const std::string handlerName = callback.target_type().name();
    gEventManager.Unsubscribe(EventType::GetStaticEventType(), handlerName, eventId);
  }

  inline void TriggerEvent(const Event& triggeredEvent, EventID eventId = 0)
  {
    gEventManager.TriggerEvent(triggeredEvent, eventId);
  }

  inline void QueueEvent(std::unique_ptr<Event>&& queuedEvent, EventID eventId = 0)
  {
    gEventManager.QueueEvent(std::forward<std::unique_ptr<Event>>(queuedEvent), eventId);
  }
}
