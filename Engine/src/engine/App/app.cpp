#include "APE_engine.hpp"
#include <memory>

int main() {
  // Asimov Physics Engine (A.P.E.)
  // Make a new app ptr
  std::unique_ptr<Engine> Asimov(new Engine("A.P.E."));
	Asimov->Run();
	Asimov->Clean();
}
