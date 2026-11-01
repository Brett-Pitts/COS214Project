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
	std::string profession;
	std::vector<EmployeeComponent*> members;
	// for gui:
	void relink();
	bool canTake(WorkUnit* workUnit);

public:
	Team(std::string name, std::string profession);

	const std::string& getProfession();

	bool addMember(EmployeeComponent* employeeComponent);
	// for gui:
	int getMemberCount();
	EmployeeComponent* getMember(int index);
	bool removeMember(int index);
	bool replaceMember(int index, EmployeeComponent* replacement);

	void assignWork(WorkUnit* workUnit);
	// for gui:
	bool stepWork(WorkUnit* workUnit);
	bool retryOrFail(WorkUnit* workUnit);

	void notifyColleagues(EmployeeComponent* employeeComponent, std::string message) override;

	void add(CompanyNode* companyNode) {};

	void remove(CompanyNode* companyNode) {};

	CompanyNode* getChild(int n) { return nullptr; };

	virtual CompanyNode* clone();

	~Team();
};

#endif
