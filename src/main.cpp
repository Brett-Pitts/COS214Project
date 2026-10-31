#include "ApplicationInterface.h"
#include <iostream>

int main(int argc, char** argv) {
	ApplicationInterface interface;
	std::cout << "Welcome to TIVIDY\n";

	if (argc > 0) {
		if (std::string(argv[1]) == "ScenarioA") {
			std::cout << "Loading scenario A\n";
			interface.createScenarioA();

		}
		else if (std::string(argv[1]) == "ScenarioB") {
			std::cout << "Loading scenario B\n";
			interface.createScenarioB();
		}
	}
}