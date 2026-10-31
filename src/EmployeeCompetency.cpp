#include "EmployeeCompetency.h"
#include "WorkUnit.h"

#include <iostream>

EmployeeCompetency::EmployeeCompetency(CompetencyLevel level) : EmployeeDetail(), competencyId(level) {}

EmployeeCompetency::EmployeeCompetency(const EmployeeCompetency& other) : EmployeeDetail(other), competencyId(other.competencyId) {}

std::string EmployeeCompetency::getCompetency() {
	return this->getCompetencyName(this->competencyId);
}

EmployeeCompetency::~EmployeeCompetency() {}

std::string EmployeeCompetency::getKind() {
	return "Competency";
}

CompetencyLevel EmployeeCompetency::getCompetencyLevel(const std::string& name) {

	if (name == "Intern") {
		return INTERN;
	}
	if (name == "Junior") {
		return JUNIOR;
	}
	if (name == "Senior") {
		return SENIOR;
	}
	return NONE;
}

std::string EmployeeCompetency::getCompetencyName(CompetencyLevel level) {

	if (level == INTERN) {
		return "Intern";
	}
	if (level == JUNIOR) {
		return "Junior";
	}
	if (level == SENIOR) {
		return "Senior";
	}
	return "";
}

bool EmployeeCompetency::compareCompetency(CompetencyLevel requiredCompetency) {
	if (requiredCompetency >= 0 && competencyId >= requiredCompetency) {
		return true;
	}
	return EmployeeDetail::compareCompetency(requiredCompetency);
}

void EmployeeCompetency::prepare(WorkUnit* workUnit) {
	std::cout << getName() << " gathers " << this->getCompetencyName(this->competencyId) << " tools for " << workUnit->getName() << std::endl;
}

void EmployeeCompetency::perform(WorkUnit* workUnit) {
	std::cout << getName() << " uses " << this->getCompetencyName(this->competencyId) << " skills on " << workUnit->getName() << std::endl;
}

void EmployeeCompetency::finish(WorkUnit* workUnit) {
	std::cout << getName() << " records " << this->getCompetencyName(this->competencyId) << " outcome for " << workUnit->getName() << std::endl;
}

EmployeeComponent* EmployeeCompetency::clone() {
	EmployeeComponent* ret = new EmployeeCompetency(competencyId);
	if (wrapped != nullptr) {
		ret->wrap(wrapped->clone());
	}
	return ret;
}
