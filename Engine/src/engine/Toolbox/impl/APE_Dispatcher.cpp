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

void Dispatcher::KeySubscriber(EventType event, KeyHandler handler) {
  keyHandlers[event].push_back(std::move(handler));
}

void Dispatcher::KeyEmitter(EventType event,
                            std::variant<KeyPress, ModKeys> keypress) {

  auto iter = keyHandlers.find(event);
  if (iter == keyHandlers.end())
    return;

  for (auto &handler : iter->second) {
    handler(keypress);
  }
}
