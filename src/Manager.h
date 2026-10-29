#ifndef MANAGER_H
#define MANAGER_H

#include "Employee.h"

class Project;

class Manager : public Employee {

public:
	Manager(std::string name);
	Manager(const Manager& other);
	virtual ~Manager();

	std::string getKind();
	void handleEscalation(WorkUnit* workUnit);
	void cancelProject(Project* project);
	virtual EmployeeComponent* clone();
};

#endif
