#include "EmployeeComponent.h"
#include "Mediator.h"
#include "WorkUnit.h"
#include "EmployeeCompetency.h"
#include "CompetencyLevel.h"
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
// for gui:


EmployeeComponent* EmployeeComponent::getCore() {
	return this;
}
bool EmployeeComponent::compareProfession(string ) {
	return false;
}
bool EmployeeComponent::compareCompetency(CompetencyLevel ) {
	return false;
}
std::string EmployeeComponent::getProfession() {
	return "";
}
std::string EmployeeComponent::getCompetency() {
	return "";
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
		return compareCompetency(CompetencyLevel::INTERN);
	}
	if (workUnit->isOngoing()) {
		return compareCompetency(CompetencyLevel::JUNIOR);
	}
	if (workUnit->getStateName() == "Escalated") {
		return compareCompetency(CompetencyLevel::SENIOR);
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
	std::cout << "No employee in the chain can handle " << workUnit->getName() << std::endl;
	return false;
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
