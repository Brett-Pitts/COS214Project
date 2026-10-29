#include "Manager.h"
#include "WorkUnit.h"

#include <iostream>

Manager::Manager(std::string name) : Employee(name) {}

Manager::Manager(const Manager& other) : Employee(other) {}

Manager::~Manager() {}

std::string Manager::getKind() {
	return "Manager";
}

void Manager::handleEscalation(WorkUnit* workUnit) {
	if (workUnit == nullptr) {
		return;
	}
	std::cout << getName() << " handles escalation of " << workUnit->getName() << std::endl;
}

void Manager::cancelProject(Project* project) {
	if (project == nullptr) {
		return;
	}
	std::cout << getName() << " cancels project" << std::endl;
}

EmployeeComponent* Manager::clone() {
	EmployeeComponent* ret = new Manager(this->getName());

	return ret;
}
