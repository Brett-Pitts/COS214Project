#ifndef CONCRETEEMPLOYEEBUILDER_H
#define CONCRETEEMPLOYEEBUILDER_H

#include "EmployeeBuilder.h"


class ConcreteEmployeeBuilder : public EmployeeBuilder {


public:
	void reset();
	bool addCompetency(std::string name);

	bool addProfession(std::string name);


	EmployeeComponent* finish(std::string name, bool isManager = false);
};

#endif
