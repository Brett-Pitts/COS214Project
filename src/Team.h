#ifndef TEAM_H
#define TEAM_H

#include <string>
#include <vector>
#include "CompanyNode.h"
#include "Mediator.h"

class FileSystem;
class EmployeeComponent;
class WorkUnit;

/**
 * @class Team
 * @brief A group of employees of one profession; the leaf of the company tree.
 *
 * Work is given to the team, which passes it along its members in the order
 * they were added until one can do it. The team also tells the other members
 * when someone has worked on a unit.
 *
 * @par Design patterns
 * - Composite (Leaf): a Team has no child nodes, so add(), remove() and
 *   getChild() do nothing.
 * - Mediator (ConcreteMediator): members are the colleagues.
 * - Chain of Responsibility: members are linked through setSuccessor() in
 *   addMember().
 *
 * @see CompanyNode, Mediator, EmployeeComponent
 */
class Team : public CompanyNode, public Mediator {
	friend class FileSystem;
private:
	std::string profession;                    ///< Profession of every member of this team.
	std::vector<EmployeeComponent*> members;   ///< Members, in the order they were added. Owned.

public:
	/**
	 * @brief Creates a team.
	 * @param profession Profession of the team. Defaults to an empty string.
	 */
	Team(std::string profession = "");

	/** @brief Gets the profession of this team. */
	std::string getProfession();

	/**
	 * @brief Adds an employee to the team.
	 *
	 * Sets the employee's mediator to this team and links it as the successor
	 * of the previous member, so that the chain and mediator are set up in
	 * one place.
	 *
	 * @param employeeComponent The employee to add. Ownership passes to the team.
	 * @return true if the employee was added, false if it was rejected.
	 */
	bool addMember(EmployeeComponent* employeeComponent);

	/**
	 * @brief Gives a work unit to the team.
	 *
	 * Checks the profession and passes the unit to the first member's
	 * handleWork(). If nobody can take it, the retry behaviour of the work
	 * unit applies.
	 *
	 * @param workUnit The unit to assign.
	 */
	void assignWork(WorkUnit* workUnit);

	/**
	 * @brief Tells the other members that one member has worked on a unit.
	 * @param employeeComponent The member who did the work (not notified).
	 * @param message The message to deliver.
	 */
	void notifyColleagues(EmployeeComponent* employeeComponent, std::string message) override;

	/** @brief Does nothing. A team is a leaf and has no child nodes. */
	void add(CompanyNode* companyNode) {};

	/** @brief Does nothing. A team is a leaf and has no child nodes. */
	void remove(CompanyNode* companyNode) {};

	/**
	 * @brief Always returns nullptr. A team is a leaf and has no child nodes.
	 * @param n Ignored.
	 */
	CompanyNode* getChild(int n) {return nullptr; };

	/**
	 * @brief Makes a deep copy of this team and its members.
	 * @return A newly allocated copy. The caller owns it.
	 */
	virtual CompanyNode* clone();

	/** @brief Destroys the team and its members. */
	~Team();
};

#endif
