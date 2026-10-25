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



bool EmployeeDirector::addProfession(int id) {
	if (builder == nullptr) {
		std::cerr << "Builder has not been set.\n";
		return false;
	}
	return builder->addProfession(id);
}

bool EmployeeDirector::addCompetency(int id) {
	if (builder == nullptr) {
		std::cerr << "Builder has not been set.\n";
		return false;
	}
	return builder->addCompetency(id);
}

EmployeeComponent* EmployeeDirector::finish(std::string name, bool isManager) {
	if (builder == nullptr) {
		std::cerr << "Builder has not been set.\n";
		return nullptr;
	}
	return builder->finish(name, isManager);
}
