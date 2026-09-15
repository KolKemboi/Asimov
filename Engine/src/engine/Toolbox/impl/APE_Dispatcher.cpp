#include <APE_Dispatcher.hpp>
#include <utility>

void Dispatcher::Subscriber(EventType event, Handler handler) {
  handlers[event].push_back(std::move(handler));
}

void Dispatcher::Emitter(EventType event, const std::string &data = {}) {
  auto iter = handlers.find(event);
  if (iter == handlers.end())
    return;

  for (auto &handler : iter->second) {
    handler(data);
  }
}
/*
 * there is a struct, with A, and thing with mod A
 * if with mod, call x, if without mod, call X
 *
 *
 */
