#include "EmployeeCompetency.h"
#include "WorkUnit.h"

#include <iostream>

EmployeeCompetency::EmployeeCompetency(string name) : EmployeeDetail(), competencyId(getCompetencyLevel(name)), competencyName(name) {}

EmployeeCompetency::EmployeeCompetency(const EmployeeCompetency& other) : EmployeeDetail(other), competencyId(other.competencyId), competencyName(other.competencyName) {}

EmployeeCompetency::~EmployeeCompetency() {}

std::string EmployeeCompetency::getKind() {
	return "Competency";
}

int EmployeeCompetency::getCompetencyLevel(string name) {
	if (name == "intern") {
		return INTERN;
	}
	if (name == "junior") {
		return JUNIOR;
	}
	if (name == "senior") {
		return SENIOR;
	}
	return -1;
}

bool EmployeeCompetency::compareCompetency(int requiredCompetency) {
	if (requiredCompetency >= 0 && competencyId >= requiredCompetency) {
		return true;
	}
	return EmployeeDetail::compareCompetency(requiredCompetency);
}

void EmployeeCompetency::prepare(WorkUnit* workUnit) {
	cout << getName() << " gathers " << competencyName << " tools for " << workUnit->getName() << endl;
}

void EmployeeCompetency::perform(WorkUnit* workUnit) {
	cout << getName() << " uses " << competencyName << " skills on " << workUnit->getName() << endl;
}

void EmployeeCompetency::finish(WorkUnit* workUnit) {
	cout << getName() << " records " << competencyName << " outcome for " << workUnit->getName() << endl;
}

EmployeeComponent* EmployeeCompetency::clone() {
	EmployeeComponent* ret = new EmployeeCompetency(competencyName);
	if (wrapped != nullptr) {
		ret->wrap(wrapped->clone());
	}
	return ret;
}
