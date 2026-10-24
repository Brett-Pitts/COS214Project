#ifndef MANAGER_H
#define MANAGER_H

class Manager : Employee {


public:
	void handleEscalation(WorkUnit workUnit);

	void cancelProject(Project project);

	EmployeeComponent* clone();
};

#endif
