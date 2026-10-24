#ifndef EMPLOYEEBUILDER_H
#define EMPLOYEEBUILDER_H

class EmployeeBuilder {

private:
	EmployeeComponent employee;

public:
	void addCompetency(int id);

	void addProfession(int id);

	EmployeeComponent* constructEmployee();

	void build(string name, boolean isManager = false);
};

#endif
