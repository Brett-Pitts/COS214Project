#include "Team.h"
#include "EmployeeComponent.h"

void Team::addMember(EmployeeComponent* employeeComponent) {
	int count = this->members.size();
	if (count > 0) {
		this->members[count - 1]->setSuccessor(employeeComponent);
	}
	employeeComponent->setMediator(this);
	this->members.push_back(employeeComponent);
}

void Team::assignWork(WorkUnit* workUnit) {
	if (this->members.empty()) {
		return;
	}
	this->members[0]->handleWork(workUnit);
}

void Team::notifyColleagues(EmployeeComponent* employeeComponent, std::string message) {
	int count = this->members.size();

	for (int i = 0; i < count; i++) {
		if (this->members[i] != employeeComponent) {
			this->members[i]->receive(message);
		}
	}
}

CompanyNode* Team::clone() {
	Team* ret = new Team();

	for (auto& mem : members) {
		ret->addMember(mem->clone());
	}

	return ret;
}

Team::~Team() {
	if (!members.empty()) {
		for (auto it = members.begin(); it != members.end(); ++it) {

			delete* it;
		}
	}
}
