#pragma once

#include "APE_Dispatcher.hpp"
class RobotMaker {
public:
  RobotMaker(Dispatcher &dispatcher) {
    dispatcher.Subscriber(
        EventType::ROBOTLOADED,
        [this](const std::string &robotPath) { this->loadRobot(robotPath); });
  }

private:
  void loadRobot(const std::string &);
};
