#ifndef CONCRETEEMPLOYEEBUILDER_H
#define CONCRETEEMPLOYEEBUILDER_H

#include "EmployeeBuilder.h"


class ConcreteEmployeeBuilder : public EmployeeBuilder {


public:
	void reset();
	bool addCompetency(int id);

	bool addProfession(int id);


	EmployeeComponent* finish(std::string name, bool isManager = false);
};

#endif
