#ifndef EMPLOYEE_H
#define EMPLOYEE_H

class Employee : EmployeeComponent {

public:
	string name;

	void doWork(WorkUnit workUnit);

	void wrap(EmployeeComponent component);

	EmployeeComponent* clone();
};

#endif
