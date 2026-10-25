#ifndef EMPLOYEEBUILDER_H
#define EMPLOYEEBUILDER_H

#include "EmployeeComponent.h"
#include <string>

class EmployeeBuilder {

protected:
	EmployeeComponent* employee;

public:
	virtual void reset() = 0;
	virtual bool addCompetency(int id) = 0;

	virtual bool addProfession(int id) = 0;


	virtual EmployeeComponent* finish(std::string name, bool isManager = false) = 0;
};

#endif
