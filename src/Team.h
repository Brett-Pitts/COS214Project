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
	/**
	 * @brief Creates a team for a profession.
	 * @param name Team name.
	 * @param profession The profession.
	 */
	Team(std::string name, std::string profession);

	/**
	 * @brief Gets the team's profession.
	 * @return The team's profession.
	 */
	const std::string& getProfession();

	/**
	 * @brief Adds a member to the end of the chain; false if null or the wrong profession.
	 * @param employeeComponent The member to add (now owned).
	 * @return True if added.
	 */
	bool addMember(EmployeeComponent* employeeComponent);
	// for gui:
	int getMemberCount();
	EmployeeComponent* getMember(int index);
	bool removeMember(int index);
	bool replaceMember(int index, EmployeeComponent* replacement);

	/**
	 * @brief Steps the unit along as far as the team can, or retries/fails it.
	 * @param workUnit The unit being worked on.
	 */
	void assignWork(WorkUnit* workUnit);
	// for gui:
	bool stepWork(WorkUnit* workUnit);
	bool retryOrFail(WorkUnit* workUnit);

	/**
	 * @brief Sends a message to every member except the sender.
	 * @param employeeComponent The sender.
	 * @param message The message text.
	 */
	void notifyColleagues(EmployeeComponent* employeeComponent, std::string message) override;

	/**
	 * @brief Does nothing, a team has no child nodes.
	 * @param companyNode Ignored.
	 */
	void add(CompanyNode* companyNode) {};

	/**
	 * @brief Does nothing, a team has no child nodes.
	 * @param companyNode Ignored.
	 */
	void remove(CompanyNode* companyNode) {};

	/**
	 * @brief Always nullptr, a team has no children.
	 * @param n Ignored.
	 * @return Always nullptr.
	 */
	CompanyNode* getChild(int n) { return nullptr; };

	/**
	 * @brief Makes a deep copy of the team and its members.
	 * @return A new copy; the caller owns it.
	 */
	virtual CompanyNode* clone();

	/** @brief Deletes the team's members. */
	~Team();
};

#endif
