#ifndef TEAM_H
#define TEAM_H

class Team : CompanyNode, Mediator {

public:
	std::vector<EmployeeComponent> members;

	void addMember(EmployeeComponent employeeComponent);

	void assignWork(WorkUnit workUnit);

	void notifyColleagues(EmployeeComponent employeeComponent, string arg1);
};

#endif
