#include "EmployeeProfession.h"
#include "WorkUnit.h"

#include <iostream>

std::vector<std::string> EmployeeProfession::professionList;

EmployeeProfession::EmployeeProfession(std::string name) : EmployeeDetail(), professionName(registerProfession(name)) {}

EmployeeProfession::EmployeeProfession(const EmployeeProfession& other) : EmployeeDetail(other), professionName(other.professionName) {}

EmployeeProfession::~EmployeeProfession() {}

std::string EmployeeProfession::getKind() {
	return "Profession";
}

std::string EmployeeProfession::registerProfession(std::string name) {
	if (name.empty()) {
		return "";
	}
	if (hasProfession(name)) {
		return name;
	}

	professionList.push_back(name);
	return name;
}

bool EmployeeProfession::deleteProfession(std::string name) {
	if (!hasProfession(name)) {
		return false;
	}
	std::vector<std::string>::iterator iter = professionList.begin();
	for (;*iter != name;iter++);
	professionList.erase(iter);
}

bool EmployeeProfession::hasProfession(std::string name) {
	for (int i = 0;i < professionList.size();i++) {
		if (professionList[i] == name) {
			return true;
		}
	}
	return false;
}

bool EmployeeProfession::compareProfession(std::string professionName) {
	if (this->professionName == professionName) {
		return true;
	}
	return wrapped->compareProfession(professionName);
}

void EmployeeProfession::prepare(WorkUnit* workUnit) {
	std::cout << getName() << " reviews " << workUnit->getName() << " as " << professionName << std::endl;
}

void EmployeeProfession::perform(WorkUnit* workUnit) {
	std::cout << getName() << " applies " << professionName << " expertise to " << workUnit->getName() << std::endl;
}

void EmployeeProfession::finish(WorkUnit* workUnit) {
	std::cout << getName() << " signs off " << workUnit->getName() << " as " << professionName << std::endl;
}


EmployeeComponent* EmployeeProfession::clone() {
	EmployeeComponent* ret = new EmployeeProfession(professionName);
  if(wrapped!=nullptr)
	  ret->wrap(wrapped->clone());
	return ret;

}