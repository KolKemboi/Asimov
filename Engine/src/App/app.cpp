#include "APE_window.hpp"
#include <memory>

int main() {
  // Asimov Physics Engine (A.P.E.)
  // Make a new app ptr
  std::unique_ptr<APE_Window> Asimov(new APE_Window(1920, 1080, "A.P.E."));
  Asimov->RunEngine();
  Asimov->CleanUp();
}
