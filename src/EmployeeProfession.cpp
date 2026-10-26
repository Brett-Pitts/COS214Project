#include "EmployeeProfession.h"
#include "WorkUnit.h"

#include <iostream>

vector<string> EmployeeProfession::professionList;

EmployeeProfession::EmployeeProfession(string name) : EmployeeDetail(), professionId(registerProfession(name)) {
}

EmployeeProfession::EmployeeProfession(const EmployeeProfession& other) : EmployeeDetail(other), professionId(other.professionId) {
}

EmployeeProfession::~EmployeeProfession() {
}

int EmployeeProfession::registerProfession(string name) {
	if (name.empty()) {
		return -1;
	}
	int existing = getProfessionId(name);
	if (existing >= 0) {
		return existing;
	}
	for (size_t i = 0; i < professionList.size(); i++) {
		if (professionList[i].empty()) {
			professionList[i] = name;
			return (int) i;
		}
	}
	professionList.push_back(name);
	return (int) professionList.size() - 1;
}

bool EmployeeProfession::deleteProfession(string name) {
	int id = getProfessionId(name);
	if (id < 0) {
		return false;
	}
	professionList[id] = "";
	return true;
}

int EmployeeProfession::getProfessionId(string name) {
	if (name.empty()) {
		return -1;
	}
	for (size_t i = 0; i < professionList.size(); i++) {
		if (professionList[i] == name) {
			return (int) i;
		}
	}
	return -1;
}

bool EmployeeProfession::compareProfession(int professionId) {
	if (this->professionId == professionId) {
		return true;
	}
	return EmployeeDetail::compareProfession(professionId);
}

void EmployeeProfession::prepare(WorkUnit* workUnit) {
	cout << getName() << " reviews " << workUnit->getName() << " as " << professionList[professionId] << endl;
}

void EmployeeProfession::perform(WorkUnit* workUnit) {
	cout << getName() << " applies " << professionList[professionId] << " expertise to " << workUnit->getName() << endl;
}

void EmployeeProfession::finish(WorkUnit* workUnit) {
	cout << getName() << " signs off " << workUnit->getName() << " as " << professionList[professionId] << endl;
}

EmployeeComponent* EmployeeProfession::clone() {
	return new EmployeeProfession(*this);
}
