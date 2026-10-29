#include "Team.h"
#include "EmployeeComponent.h"
#include "WorkUnit.h"
#include "EmployeeProfession.h"

Team::Team(int profession) {
	this->profession = profession;
}

int Team::getProfession() {
	return this->profession;
}

bool Team::addMember(EmployeeComponent* employeeComponent) {
	if (employeeComponent == nullptr) {
		return false;
	}
	if (!employeeComponent->compareProfession(this->profession)) {
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

	std::string requiredName = workUnit->getRequiredProfession();
	if (requiredName != "") {
		int required = EmployeeProfession::getProfessionId(requiredName);
		if (required < 0 || (this->profession >= 0 && required != this->profession)) {
			return;
		}
	}

	while (this->members[0]->handleWork(workUnit)) {
	}
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

Team::~Team()
{
	if (!members.empty()) {
		for (auto it = members.begin(); it != members.end(); ++it) {
			
			delete *it;
		}
	}
}
