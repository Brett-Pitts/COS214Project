#ifndef EMPLOYEEDIRECTOR_H
#define EMPLOYEEDIRECTOR_H

#include <string>
#include "EmployeeComponent.h"
#include "EmployeeBuilder.h"



class EmployeeDirector {
private:
	EmployeeBuilder* builder;

public:
	EmployeeDirector() : builder(nullptr) {}

	void setBuilder(EmployeeBuilder* builder);


	void reset();
	bool addProfession(std::string name);

	bool addCompetency(std::string name);

	EmployeeComponent* finish(std::string name, bool isManager = false);
};

#endif
