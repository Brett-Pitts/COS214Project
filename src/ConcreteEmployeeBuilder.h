#ifndef CONCRETEEMPLOYEEBUILDER_H
#define CONCRETEEMPLOYEEBUILDER_H

#include "EmployeeBuilder.h"


class ConcreteEmployeeBuilder : public EmployeeBuilder {


public:
	void reset();
	void addCompetency(int id);

	void addProfession(int id);


	EmployeeComponent* build(std::string name, bool isManager = false);
};

#endif
