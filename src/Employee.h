#ifndef EMPLOYEE_H
#define EMPLOYEE_H

class Employee : EmployeeComponent {

public:
	std::string name;

	void doWork(WorkUnit workUnit);

	void wrap(EmployeeComponent component);

	EmployeeComponent* clone();
};

#endif
