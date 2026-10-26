#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "EmployeeComponent.h"

class Employee : public EmployeeComponent {

private:
	string name;

public:
	Employee(string name);
	Employee(const Employee& other);
	virtual ~Employee();

	string getName();
	void doWork(WorkUnit* workUnit);
	void wrap(EmployeeComponent* component);
	EmployeeComponent* clone();
};

#endif
