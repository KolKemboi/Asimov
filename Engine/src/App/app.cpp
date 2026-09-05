#include "APE_window.hpp"

int main() {
  // Asimov Physics Engine (A.P.E.)
	// Use the stack when possible
  APE_Window Asimov = APE_Window(960, 720, "A.P.E.");
  Asimov.RunEngine();
  Asimov.CleanUp();
}
