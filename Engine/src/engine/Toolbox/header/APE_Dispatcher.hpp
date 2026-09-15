#pragma once

#include <APE_KeyEvents.hpp>
#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

// add more events
enum class EventType {
  OBJECT_ADDED,
  OBJECT_MODIFIED,
  COLLIDER_TYPE_MODIFIED,
  KEYPRESS,
};

class Dispatcher {
public:
  // this handler takes a function ptr that returns nothing and the input is a
  // string
  using Handler = std::function<void(const std::string &)>;

  // listen for input event, and the event be A with shift mod
  // so emitter should be, event type, key pressed

  // Subscriber and Emitter
  void Subscriber(EventType, Handler);
  void Emitter(EventType, const std::string &);


private:
  std::unordered_map<EventType, std::vector<Handler>> handlers;
};
