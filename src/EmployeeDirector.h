#ifndef EMPLOYEEDIRECTOR_H
#define EMPLOYEEDIRECTOR_H

#include "EmployeeComponent.h"

class EmployeeDirector {

public:
	boolean addProfession(string id);
public:

	boolean addCompetency(string name);
private:

	bool finish(string name, bool isManager = false);
public:

	EmployeeComponent* finish(string name, boolean isManager = false);

	void setBuilder(EmployeeBuilder* builder);

	void reset();

	boolean addProfession(string id);

	boolean addCompetency(string name);
private:

	EmployeeBuilder* builder;
};

#endif
