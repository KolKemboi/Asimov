#include "APE_window.hpp"
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

#ifdef DEBUG__
#pragma message("DEBUG__ is defined")
#else
#pragma message("DEBUG__ is NOT defined")
#endif

int main() {
#ifdef DEBUG__
  spdlog::set_level(spdlog::level::debug);
#else
  spdlog::set_level(spdlog::level::warn);
#endif
  // Asimov Physics Engine (A.P.E.)
  // Use the stack when possible
  // APE_Window Asimov = APE_Window(960, 720, "A.P.E.");
  APE_Window Asimov = APE_Window(1920, 1080, "A.P.E.");
  Asimov.RunEngine(); // call engine run cycle
  Asimov.CleanUp();   // on exit, delete every resource
}
