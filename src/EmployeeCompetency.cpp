#include "EmployeeCompetency.h"
#include "WorkUnit.h"

#include <iostream>

std::vector<std::string> EmployeeCompetency::competencyList;

EmployeeCompetency::EmployeeCompetency(std::string name) : EmployeeDetail(), competencyName(registerCompetency(name)) {}

EmployeeCompetency::EmployeeCompetency(const EmployeeCompetency& other) : EmployeeDetail(other), competencyName(other.competencyName) {}

EmployeeCompetency::~EmployeeCompetency() {}

std::string EmployeeCompetency::getKind() {
	return "Competency";
}

std::string EmployeeCompetency::registerCompetency(std::string name) {
	if (name.empty()) {
		return "";
	}
	if (hasCompetency(name)) {
		return name;
	}

	competencyList.push_back(name);
	return name;
}

bool EmployeeCompetency::deleteCompetency(std::string name) {
	if (!hasCompetency(name)) {
		return false;
	}
	std::vector<std::string>::iterator iter = competencyList.begin();
	for (;*iter != name;iter++);
	competencyList.erase(iter);
}

bool EmployeeCompetency::hasCompetency(std::string name) {
	for (int i = 0;i < competencyList.size();i++) {
		if (competencyList[i] == name) {
			return true;
		}
	}
	return false;
}

void EmployeeCompetency::prepare(WorkUnit* workUnit) {
	std::cout << getName() << " gathers " << competencyName << " tools for " << workUnit->getName() << std::endl;
}

void EmployeeCompetency::perform(WorkUnit* workUnit) {
	std::cout << getName() << " uses " << competencyName << " on " << workUnit->getName() << std::endl;
}

void EmployeeCompetency::finish(WorkUnit* workUnit) {
	std::cout << getName() << " records " << competencyName << " outcome for " << workUnit->getName() << std::endl;
}

EmployeeComponent* EmployeeCompetency::clone() {
	EmployeeComponent* ret = new EmployeeCompetency(competencyName);
	ret->wrap(wrapped->clone());
	return ret;
}
