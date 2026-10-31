#include "Team.h"
#include "EmployeeComponent.h"
#include "WorkUnit.h"
#include "EmployeeProfession.h"
#include "Failed.h"
#include <iostream>
Team::Team(std::string profession) {
	this->profession = profession;
}

std::string Team::getProfession() {
	return this->profession;
}

bool Team::addMember(EmployeeComponent* employeeComponent) {
	if (employeeComponent == nullptr) {
		return false;
	}
	if (!this->profession.empty() && !employeeComponent->compareProfession(this->profession)) {
		return false;
	}
	int count = this->members.size();
	if (count > 0) {
		this->members[count - 1]->setSuccessor(employeeComponent);
	}
	employeeComponent->setMediator(this);
	this->members.push_back(employeeComponent);
	return true;
}
void Team::assignWork(WorkUnit* workUnit) {
	if (this->members.empty() || workUnit == nullptr) {
		return;
	}

	std::string required = workUnit->getRequiredProfession();
	if (required != "") {
		if (!EmployeeProfession::hasProfession(required)) {
			return;
		}
		if (!this->profession.empty() && required != this->profession) {
			return;
		}
	}

	bool advanced = false;
	while (this->members[0]->handleWork(workUnit)) {
		advanced = true;
	}

	if (!advanced && (workUnit->isReady() || workUnit->isOngoing() || workUnit->getStateName() == "Escalated")) {
		if (workUnit->hasRetriesLeft()) {
			workUnit->reset();
		}
		else {
			workUnit->setState(new Failed());
		}
	}
}

void Team::notifyColleagues(EmployeeComponent* employeeComponent, std::string message) {
	int count = this->members.size();

	for (int i = 0; i < count; i++) {
		if (this->members[i]->getCore() != employeeComponent->getCore()) {
			this->members[i]->receive(message);
		}
	}
}

CompanyNode* Team::clone() {
	Team* copy = new Team(this->profession);
	int count = this->members.size();

	for (int i = 0; i < count; i++) {
		EmployeeComponent* memberCopy = this->members[i]->clone();
		if (!copy->addMember(memberCopy)) {
			delete memberCopy;
		}
	}
	return copy;
}

Team::~Team() {

	if (!members.empty()) {
		for (auto it = members.begin(); it != members.end(); ++it) {

			delete* it;
		}
	}
}
