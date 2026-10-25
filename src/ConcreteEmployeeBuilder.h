#ifndef CONCRETEEMPLOYEEBUILDER_H
#define CONCRETEEMPLOYEEBUILDER_H

class ConcreteEmployeeBuilder : EmployeeBuilder {


public:

	void build(string name, bool isManager = false);

	EmployeeComponent* finish(string name, boolean isManager = false);

	void reset();

	boolean addCompetency(string name);

	boolean addProfession(string name);
};

#endif
