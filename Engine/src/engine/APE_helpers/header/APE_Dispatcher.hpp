#pragma once

#include <functional>
#include <string>
#include <unordered_map>
#include <vector>

enum class EventType {
  OBJECT_ADDED,
  OBJECT_MODIFIED,
  COLLIDER_TYPE_MODIFIED,

};

class Dispatcher {
public:
  using Handler = std::function<void(const std::string &)>;

  void Subscriber(EventType, Handler);
  void Emitter(EventType, const std::string &);

private:
  std::unordered_map<EventType, std::vector<Handler>> handlers;
};
