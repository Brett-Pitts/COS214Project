#ifndef EMPLOYEEBUILDER_H
#define EMPLOYEEBUILDER_H

class EmployeeBuilder {

protected:
	EmployeeComponent employee;

public:
	virtual boolean addCompetency(string name) = 0;

	virtual boolean addProfession(string name) = 0;

	void build(string name, bool isManager = false);

	virtual EmployeeComponent* finish(string name, boolean isManager = false) = 0;

	virtual void reset() = 0;
};

#endif
