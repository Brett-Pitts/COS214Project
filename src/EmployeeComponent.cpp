#include "EmployeeComponent.h"
#include "Mediator.h"
#include "WorkUnit.h"

#include <iostream>

EmployeeComponent::EmployeeComponent() : mediator(nullptr), successor(nullptr) {}

EmployeeComponent::EmployeeComponent(const EmployeeComponent& other) : mediator(other.mediator), successor(other.successor) {}

EmployeeComponent::~EmployeeComponent() {}





void EmployeeComponent::prepare(WorkUnit* workUnit) {
	std::cout << getName() << " prepares " << workUnit->getName() << std::endl;
}

void EmployeeComponent::perform(WorkUnit* workUnit) {
	std::cout << getName() << " performs " << workUnit->getName() << std::endl;
}

void EmployeeComponent::finish(WorkUnit* workUnit) {
	std::cout << getName() << " finishes " << workUnit->getName() << std::endl;
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
	std::cout << "No employee in the chain can handle " << workUnit->getName() << std::endl;
}

void EmployeeComponent::receive(std::string message) {
	std::cout << getName() << " received: " << message << std::endl;
}

void EmployeeComponent::setSuccessor(EmployeeComponent* employeeComponent) {
	this->successor = employeeComponent;
}

void EmployeeComponent::setMediator(Mediator* mediator) {
	this->mediator = mediator;
}

