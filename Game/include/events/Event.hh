#pragma once
#include <string>
#include <iostream>
#include <cstdint>
#include "core/CRC32.h"

namespace Events {
  class Event {
  public:
    virtual ~Event() = default;
    virtual std::uint32_t GetEventType() const = 0;
    virtual std::string ToString() const { return std::to_string(GetEventType()); }

    bool m_IsHandled { false };

  };
  #define EVENT_TYPE(event_type) \
    static std::uint32_t GetStaticEventType() \
    { \
      return CRC32(#event_type); \
    } \
    std::uint32_t GetEventType() const override \
    { \
      return GetStaticEventType(); \
    }


  inline std::ostream& operator<<(std::ostream& os, const Event& e)
  {
    return os << e.ToString();
  }
}
