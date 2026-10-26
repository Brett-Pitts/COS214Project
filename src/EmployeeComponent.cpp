#include "EmployeeComponent.h"
#include "Mediator.h"
#include "WorkUnit.h"

#include <iostream>

EmployeeComponent::EmployeeComponent() : mediator(nullptr), successor(nullptr) {}

EmployeeComponent::EmployeeComponent(const EmployeeComponent& other) : mediator(other.mediator), successor(other.successor) {}

EmployeeComponent::~EmployeeComponent() {}

bool EmployeeComponent::compareProfession(int professionId) {
	return professionId < 0;
}

void EmployeeComponent::prepare(WorkUnit* workUnit) {
	cout << getName() << " prepares " << workUnit->getName() << endl;
}

void EmployeeComponent::perform(WorkUnit* workUnit) {
	cout << getName() << " performs " << workUnit->getName() << endl;
}

void EmployeeComponent::finish(WorkUnit* workUnit) {
	cout << getName() << " finishes " << workUnit->getName() << endl;
}

void EmployeeComponent::doWork(WorkUnit* workUnit) {
	if (workUnit == nullptr) {
		return;
	}
	prepare(workUnit);
	perform(workUnit);
	finish(workUnit);
	if (mediator != nullptr) {
		mediator->notifyColleagues(this, getName() + " completed " + workUnit->getName());
	}
}

void EmployeeComponent::handleWork(WorkUnit* workUnit) {
	if (workUnit == nullptr) {
		return;
	}
	if (compareProfession(workUnit->getRequiredProfession())) {
		doWork(workUnit);
		return;
	}
	if (successor != nullptr) {
		successor->handleWork(workUnit);
		return;
	}
	cout << "No employee in the chain can handle " << workUnit->getName() << endl;
}

void EmployeeComponent::receive(string message) {
	cout << getName() << " received: " << message << endl;
}

void EmployeeComponent::setSuccessor(EmployeeComponent* employeeComponent) {
	this->successor = employeeComponent;
}

void EmployeeComponent::setMediator(Mediator* mediator) {
	this->mediator = mediator;
}

void EmployeeComponent::setSuccessor(EmployeeComponent* employeeComponent) {
	this->successor = employeeComponent;
}