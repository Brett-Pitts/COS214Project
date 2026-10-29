#include "EmployeeComponent.h"
#include "Mediator.h"
#include "WorkUnit.h"
#include "EmployeeCompetency.h"

#include <iostream>

EmployeeComponent::EmployeeComponent() : mediator(nullptr), successor(nullptr) {}

EmployeeComponent::EmployeeComponent(const EmployeeComponent& other) : mediator(other.mediator), successor(other.successor) {}

EmployeeComponent::~EmployeeComponent() {}

bool EmployeeComponent::compareProfession(int professionId) {
	return professionId < 0;
}

bool EmployeeComponent::compareCompetency(int requiredCompetency) {
	return requiredCompetency < 0;
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
		mediator->notifyColleagues(this, getName() + " worked on " + workUnit->getName());
	}
}

bool EmployeeComponent::canAdvance(WorkUnit* workUnit) {
	if (workUnit->isReady()) {
		return compareCompetency(EmployeeCompetency::INTERN);
	}
	if (workUnit->isOngoing()) {
		return compareCompetency(EmployeeCompetency::SENIOR);
	}
	return false;
}

bool EmployeeComponent::handleWork(WorkUnit* workUnit) {
	if (workUnit == nullptr) {
		return false;
	}
	if (canAdvance(workUnit)) {
		doWork(workUnit);
		workUnit->advanceState();
		return true;
	}
	if (successor != nullptr) {
		return successor->handleWork(workUnit);
	}
	return false;
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
