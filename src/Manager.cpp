#include "Manager.h"
#include "WorkUnit.h"

#include <iostream>

Manager::Manager(string name) : Employee(name) {}

Manager::Manager(const Manager& other) : Employee(other) {}

Manager::~Manager() {}

std::string Manager::getKind() {
	return "Manager";
}

void Manager::handleEscalation(WorkUnit* workUnit) {
	if (workUnit == nullptr) {
		return;
	}
	cout << getName() << " handles escalation of " << workUnit->getName() << endl;
}

void Manager::cancelProject(Project* project) {
	if (project == nullptr) {
		return;
	}
	cout << getName() << " cancels project" << endl;
}

EmployeeComponent* Manager::clone() {
	EmployeeComponent* ret = new Manager(this->getName());

	return ret;
}
