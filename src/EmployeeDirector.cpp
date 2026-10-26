#include "EmployeeDirector.h"
#include <iostream>

void EmployeeDirector::setBuilder(EmployeeBuilder* builder) {
	if (builder == nullptr) {
		std::cerr << "Nullpointer passed to setBuilder()\n";
		return;
	}
	this->builder = builder;
}

void EmployeeDirector::reset() {
	if (builder == nullptr) {
		std::cerr << "Builder has not been set.\n";
		return;
	}

	builder->reset();

}



bool EmployeeDirector::addProfession(std::string name) {
	if (builder == nullptr) {
		std::cerr << "Builder has not been set.\n";
		return false;
	}
	return builder->addProfession(name);
}

bool EmployeeDirector::addCompetency(std::string name) {
	if (builder == nullptr) {
		std::cerr << "Builder has not been set.\n";
		return false;
	}
	return builder->addCompetency(name);
}

EmployeeComponent* EmployeeDirector::finish(std::string name, bool isManager) {
	if (builder == nullptr) {
		std::cerr << "Builder has not been set.\n";
		return nullptr;
	}
	return builder->finish(name, isManager);
}
