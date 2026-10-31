#include "EmployeeDetail.h"

EmployeeDetail::EmployeeDetail() : EmployeeComponent(), wrapped(nullptr) {}

EmployeeDetail::EmployeeDetail(const EmployeeDetail& other) : EmployeeComponent(other), wrapped(nullptr) {
	if (other.wrapped != nullptr) {
		wrapped = other.wrapped->clone();
	}
}

EmployeeDetail::~EmployeeDetail() {
	delete wrapped;
}

std::string EmployeeDetail::getName() {
	if (wrapped == nullptr) {
		return "";
	}
	return wrapped->getName();
}

bool EmployeeDetail::compareProfession(std::string professionName) {
	return wrapped->compareProfession(professionName);
}

bool EmployeeDetail::compareCompetency(int requiredCompetency) {
	if (wrapped == nullptr) {
		return EmployeeComponent::compareCompetency(requiredCompetency);
	}
	return wrapped->compareCompetency(requiredCompetency);
}

void EmployeeDetail::prepare(WorkUnit*) {}

void EmployeeDetail::perform(WorkUnit*) {}

void EmployeeDetail::finish(WorkUnit*) {}

void EmployeeDetail::doWork(WorkUnit* workUnit) {
	if (workUnit == nullptr) {
		return;
	}
	prepare(workUnit);
	perform(workUnit);
	if (wrapped != nullptr) {
		wrapped->doWork(workUnit);
	}
	finish(workUnit);
}
EmployeeComponent* EmployeeDetail::getCore() {
    if (wrapped == nullptr) {
        return this;
    }
    return wrapped->getCore();
}
void EmployeeDetail::setMediator(Mediator* mediator) {
	EmployeeComponent::setMediator(mediator);
	if (wrapped != nullptr) {
		wrapped->setMediator(mediator);
	}
}

void EmployeeDetail::wrap(EmployeeComponent* component) {
	if (component == nullptr || component == this) {
		return;
	}
	if (wrapped == nullptr) {
		wrapped = component;
	}
	else {
		wrapped->wrap(component);
	}
}


