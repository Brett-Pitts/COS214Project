#ifndef EMPLOYEEDIRECTOR_H
#define EMPLOYEEDIRECTOR_H

#include "EmployeeComponent.h"

class EmployeeDirector {


public:
	EmployeeComponent* constructEmployee();

private:
	bool addProfession(int id);

	bool addCompetency(int id);

	bool finish(string name, bool isManager = false);
};

#endif
