#ifndef TEAM_H
#define TEAM_H

#include <string>
#include <vector>
#include "CompanyNode.h"
#include "Mediator.h"

class FileSystem;
class EmployeeComponent;
class WorkUnit;

/** @brief A group of employees with one profession. Leaf of the company tree and mediator for its members. */
class Team : public CompanyNode, public Mediator {
	friend class FileSystem;
private:
	/** Profession every member needs. Empty allows any. */
	std::string profession;
	/** Members in chain order. Owned. */
	std::vector<EmployeeComponent*> members;
	// for gui:
	void relink();
	bool canTake(WorkUnit* workUnit);

public:
	/** @brief Creates a team with a name and a profession. */
	Team(std::string name, std::string profession);

	/** @brief Gets the team's profession. */
	const std::string& getProfession();

	/** @brief Adds a member to the end of the chain. False if null or the profession does not match. */
	bool addMember(EmployeeComponent* employeeComponent);
	// for gui:
	int getMemberCount();
	EmployeeComponent* getMember(int index);
	bool removeMember(int index);
	bool replaceMember(int index, EmployeeComponent* replacement);

	/** @brief Steps the unit as far as the team can. If nobody can, retries or fails it. */
	void assignWork(WorkUnit* workUnit);
	// for gui:
	bool stepWork(WorkUnit* workUnit);
	bool retryOrFail(WorkUnit* workUnit);

	/** @brief Sends a message to every member except the sender. */
	void notifyColleagues(EmployeeComponent* employeeComponent, std::string message) override;

	/** @brief Does nothing, a team has no children. */
	void add(CompanyNode* ) {};

	/** @brief Does nothing, a team has no children. */
	void remove(CompanyNode* ) {};

	/** @brief Always nullptr, a team has no children. */
	CompanyNode* getChild(int ) { return nullptr; };

	/** @brief Makes a deep copy of the team and its members. */
	virtual CompanyNode* clone();

	/** @brief Destroys the team and its members. */
	~Team();
};

#endif
