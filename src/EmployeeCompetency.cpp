#include "EmployeeCompetency.h"
#include "WorkUnit.h"

#include <iostream>

EmployeeCompetency::EmployeeCompetency(std::string name) : EmployeeDetail(), competencyId(getCompetencyLevel(name)), competencyName(name) {}

EmployeeCompetency::EmployeeCompetency(const EmployeeCompetency& other) : EmployeeDetail(other), competencyId(other.competencyId), competencyName(other.competencyName) {}

EmployeeCompetency::~EmployeeCompetency() {}

std::string EmployeeCompetency::getKind() {
	return "Competency";
}

int EmployeeCompetency::getCompetencyLevel(std::string name) {

	if (name == "Intern") {
		return INTERN;
	}
	if (name == "Junior") {
		return JUNIOR;
	}
	if (name == "Senior") {
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
	std::cout << getName() << " gathers " << competencyName << " tools for " << workUnit->getName() << std::endl;
}

void EmployeeCompetency::perform(WorkUnit* workUnit) {
	std::cout << getName() << " uses " << competencyName << " skills on " << workUnit->getName() << std::endl;
}

void EmployeeCompetency::finish(WorkUnit* workUnit) {
	std::cout << getName() << " records " << competencyName << " outcome for " << workUnit->getName() << std::endl;
}

EmployeeComponent* EmployeeCompetency::clone() {
	EmployeeComponent* ret = new EmployeeCompetency(competencyName);
	if (wrapped != nullptr) {
		ret->wrap(wrapped->clone());
	}
	return ret;
}
