#ifndef EMPLOYEEBUILDER_H
#define EMPLOYEEBUILDER_H

#include "EmployeeComponent.h"
#include <string>

class EmployeeBuilder {

protected:
	EmployeeComponent* employee;

public:
	void reset();
	void addCompetency(int id);

	void addProfession(int id);


	EmployeeComponent* build(std::string name, bool isManager = false);
};

#endif
