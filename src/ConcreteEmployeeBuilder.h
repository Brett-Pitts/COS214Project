#ifndef CONCRETEEMPLOYEEBUILDER_H
#define CONCRETEEMPLOYEEBUILDER_H

class ConcreteEmployeeBuilder : EmployeeDirector, EmployeeBuilder {


public:
	void addCompetency(int id);

	void addProfession(int id);

	EmployeeComponent* constructEmployee();

	void build(string name, boolean isManager = false);
};

#endif
