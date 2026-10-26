#include "EmployeeCompetency.h"
#include "WorkUnit.h"

#include <iostream>

vector<string> EmployeeCompetency::competencyList;

EmployeeCompetency::EmployeeCompetency(string name) : EmployeeDetail(), competencyId(registerCompetency(name)) {}

EmployeeCompetency::EmployeeCompetency(const EmployeeCompetency& other) : EmployeeDetail(other), competencyId(other.competencyId) {}

EmployeeCompetency::~EmployeeCompetency() {}

int EmployeeCompetency::registerCompetency(string name) {
	if (name.empty()) {
		return -1;
	}
	int existing = getCompetencyId(name);
	if (existing >= 0) {
		return existing;
	}
	for (size_t i = 0; i < competencyList.size(); i++) {
		if (competencyList[i].empty()) {
			competencyList[i] = name;
			return (int)i;
		}
	}
	competencyList.push_back(name);
	return (int)competencyList.size() - 1;
}

bool EmployeeCompetency::deleteCompetency(string name) {
	int id = getCompetencyId(name);
	if (id < 0) {
		return false;
	}
	competencyList[id] = "";
	return true;
}

int EmployeeCompetency::getCompetencyId(string name) {
	if (name.empty()) {
		return -1;
	}
	for (size_t i = 0; i < competencyList.size(); i++) {
		if (competencyList[i] == name) {
			return (int)i;
		}
	}
	return -1;
}

void EmployeeCompetency::prepare(WorkUnit* workUnit) {
	cout << getName() << " gathers " << competencyList[competencyId] << " tools for " << workUnit->getName() << endl;
}

void EmployeeCompetency::perform(WorkUnit* workUnit) {
	cout << getName() << " uses " << competencyList[competencyId] << " on " << workUnit->getName() << endl;
}

void EmployeeCompetency::finish(WorkUnit* workUnit) {
	cout << getName() << " records " << competencyList[competencyId] << " outcome for " << workUnit->getName() << endl;
}

EmployeeComponent* EmployeeCompetency::clone() {
	EmployeeComponent* ret = new EmployeeCompetency(competencyList[competencyId]);
	ret->wrap(wrapped->clone());
	return ret;
}
