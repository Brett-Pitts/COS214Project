#ifndef TEAM_H
#define TEAM_H

#include <string>
#include <vector>
#include "CompanyNode.h"
#include "Mediator.h"

class FileSystem;
class EmployeeComponent;
class WorkUnit;

class Team : public CompanyNode, public Mediator {
	friend class FileSystem;
private:
	std::vector<EmployeeComponent*> members;

public:
	void addMember(EmployeeComponent* employeeComponent);

	void assignWork(WorkUnit* workUnit);

	void notifyColleagues(EmployeeComponent* employeeComponent, std::string message);

	void add(CompanyNode* companyNode) {};

	void remove(CompanyNode* companyNode) {};

	CompanyNode* getChild(int n) { return nullptr; };
	virtual CompanyNode* clone();

	~Team();
};

#endif