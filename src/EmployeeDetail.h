#ifndef EMPLOYEEDETAIL_H
#define EMPLOYEEDETAIL_H

class EmployeeDetail : EmployeeComponent {

public:
	EmployeeComponent wrapped;

	void doWork(WorkUnit workUnit);

	void wrap(EmployeeComponent component);

	EmployeeComponent* clone();

	virtual static boolean addDetail(string name) = 0;

	virtual static boolean hasDetail(string name) = 0;

	virtual static boolean deleteDetail(string name) = 0;

	virtual static std::vector<std::string>& getDetails() = 0;
};

#endif
