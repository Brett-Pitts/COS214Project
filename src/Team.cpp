#include "Team.h"
#include "EmployeeComponent.h"
#include "WorkUnit.h"
#include "EmployeeProfession.h"
#include "Failed.h"
using namespace std;
Team::Team(std::string name, std::string profession) : CompanyNode(name), profession(profession) {

}

const std::string& Team::getProfession() {
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
// for gui:
void Team::relink() {
	int count = this->members.size();
	for (int i = 0; i < count; i++) {
		this->members[i]->setSuccessor(i + 1 < count ? this->members[i + 1] : nullptr);
	}
}

// for gui:
int Team::getMemberCount() {
	return this->members.size();
}

// for gui:
EmployeeComponent* Team::getMember(int index) {
	if (index < 0 || index >= (int)this->members.size()) {
		return nullptr;
	}
	return this->members[index];
}

// for gui:
bool Team::removeMember(int index) {
	if (index < 0 || index >= (int)this->members.size()) {
		return false;
	}
	EmployeeComponent* doomed = this->members[index];
	this->members.erase(this->members.begin() + index);
	this->relink();
	delete doomed;
	return true;
}

// for gui:
bool Team::replaceMember(int index, EmployeeComponent* replacement) {
	if (replacement == nullptr || index < 0 || index >= (int)this->members.size()) {
		return false;
	}
	replacement->setMediator(this);
	this->members[index] = replacement;
	this->relink();
	return true;
}

// for gui:
bool Team::canTake(WorkUnit* workUnit) {
	if (this->members.empty() || workUnit == nullptr) {
		return false;
	}

	std::string required = workUnit->getRequiredProfession();
	if (required != "") {
		if (!EmployeeProfession::hasProfession(required)) {
			return false;
		}
		if (!this->profession.empty() && required != this->profession) {
			return false;
		}
	}
	return true;
}

// for gui:
bool Team::stepWork(WorkUnit* workUnit) {
	if (!canTake(workUnit)) {
		return false;
	}
	return this->members[0]->handleWork(workUnit);
}

// for gui:
bool Team::retryOrFail(WorkUnit* workUnit) {
	if (!canTake(workUnit)) {
		return false;
	}
	if (!(workUnit->isReady() || workUnit->isOngoing() || workUnit->getStateName() == "Escalated")) {
		return false;
	}
	if (workUnit->hasRetriesLeft()) {
		workUnit->reset();
	}
	else {
		workUnit->setState(new Failed());
	}
	return true;
}

void Team::assignWork(WorkUnit* workUnit) {
	if (!canTake(workUnit)) {
		return;
	}

	bool advanced = false;
	while (stepWork(workUnit)) {
		advanced = true;
	}

	if (!advanced) {
		retryOrFail(workUnit);
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
	Team* copy = new Team(this->name, this->profession);
	copy->name = this->name;
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
