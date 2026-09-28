#pragma once

#include "APE_Dispatcher.hpp"
#include <cstring>
#include <vector>
class RobotMaker {
public:
  RobotMaker(Dispatcher &dispatcher) {
    dispatcher.Subscriber(
        EventType::ROBOTLOADED,
        [this](const std::string &robotPath) { this->loadRobot(robotPath); });
  }

private:
  void loadRobot(const std::string &);
  std::vector<std::string> Links;

  bool _checkValidity(std::string &jointName) {

    for (auto &name : Links) {
      if (std::strcmp(name.c_str(), jointName.c_str()) == 0) {
        return true;
      }
    }

    return false;
  }
};
