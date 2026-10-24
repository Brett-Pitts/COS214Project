#ifndef EMPLOYEEDETAIL_H
#define EMPLOYEEDETAIL_H

class EmployeeDetail : EmployeeComponent {

public:
	EmployeeComponent wrapped;

	void doWork(WorkUnit workUnit);

	void wrap(EmployeeComponent component);

	EmployeeComponent* clone();
};

#endif
