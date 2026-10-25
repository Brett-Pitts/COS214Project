#ifndef EMPLOYEEDIRECTOR_H
#define EMPLOYEEDIRECTOR_H

#include <string>
#include "EmployeeComponent.h"
#include "EmployeeBuilder.h"



class EmployeeDirector {
private:
	EmployeeBuilder* builder;

public:
	void setBuilder(EmployeeBuilder* builder);


	void reset();
	bool addProfession(int id);

	bool addCompetency(int id);

	EmployeeComponent* finish(std::string name, bool isManager = false);
};

#endif
