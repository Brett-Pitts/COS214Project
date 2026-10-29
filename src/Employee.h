#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "EmployeeComponent.h"

class Employee : public EmployeeComponent {

private:
	std::string name;

public:
	Employee(std::string name);
	Employee(const Employee& other);
	virtual ~Employee();

	std::string getName();
	std::string getKind();
	void doWork(WorkUnit* workUnit);
	void wrap(EmployeeComponent* component);
	virtual EmployeeComponent* clone();
	bool compareProfession(std::string professionName);
};

#endif
