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
	int profession;
	std::vector<EmployeeComponent*> members;

public:
	Team(int profession = -1);

	int getProfession();

	bool addMember(EmployeeComponent* employeeComponent);

	void assignWork(WorkUnit* workUnit);

	void notifyColleagues(EmployeeComponent* employeeComponent, std::string message) override;

	void add(CompanyNode* companyNode) {};

	void remove(CompanyNode* companyNode) {};

	CompanyNode* getChild(int n) {return nullptr; };

	virtual CompanyNode* clone();

	~Team();
};

#endif
