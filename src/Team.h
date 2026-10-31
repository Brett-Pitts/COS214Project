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
 * Work is given to the team with assignWork(). The first member is asked to
 * handle the unit; a member who cannot advance it in its current state passes it
 * to the next member (the chain is linked in addMember()). The team is also the
 * mediator of its members and tells them when someone has worked on a unit.
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
	std::string profession;                    ///< Profession of every member of this team. An empty string means the team accepts any profession.
	std::vector<EmployeeComponent*> members;   ///< Members, in the order they were added. Owned.

public:
	/**
	 * @brief Creates an empty team.
	 * @param profession Profession of the team. The default empty string lets the team accept members and work units of any profession.
	 */
	Team(std::string profession = "");

	/** @brief Gets the profession of this team. */
	std::string getProfession();

	/**
	 * @brief Adds an employee to the end of the team.
	 *
	 * The employee is rejected if it is null, or if the team has a profession and the
	 * employee does not have it (checked with compareProfession(), through the
	 * decorators). Otherwise the previous last member's successor is set to the new
	 * employee, the new employee's mediator is set to this team, and the employee is
	 * appended. The chain and the mediator are therefore set up in one place.
	 *
	 * @param employeeComponent The employee to add. The team takes ownership only if
	 *        the employee is accepted.
	 * @return true if the employee was added, false if it was rejected.
	 */
	bool addMember(EmployeeComponent* employeeComponent);

	/**
	 * @brief Gives a work unit to the team and lets the members work on it as far as they can.
	 *
	 * Does nothing if the team has no members or the unit is null. If the unit
	 * requires a profession, nothing happens unless that profession is registered
	 * (see EmployeeProfession::hasProfession()) and, for a team that has a
	 * profession, equal to the team's own.
	 *
	 * Then the first member's handleWork() is called again and again for as long as
	 * it returns true. Each call moves the unit on by one state, so one call of
	 * assignWork() can carry a unit through several states (for example from Ready to
	 * Completed) if the members have a high enough level.
	 *
	 * If no member could advance the unit at all and it is Ready, Ongoing or
	 * Escalated, one retry is used with WorkNode::reset(), which sends the unit back to
	 * Ready. When no retries are left, the unit is set to Failed instead.
	 *
	 * @param workUnit The unit to assign.
	 */
	void assignWork(WorkUnit* workUnit);

	/**
	 * @brief Tells the other members that one member has worked on a unit.
	 *
	 * Calls receive() with the message on every member except the sender. Members are
	 * compared by their core employee (getCore()), so the sender is recognised
	 * whatever decorators it is wrapped in.
	 *
	 * @param employeeComponent The member who did the work. It is not notified.
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
	 * @brief Makes a copy of this team and of all its members.
	 *
	 * The copy has the same profession. Every member is cloned and added to the copy
	 * with addMember(), so the copy has its own chain and mediator links. A cloned
	 * member that is rejected by addMember() is deleted.
	 *
	 * @return A newly allocated copy. The caller owns it.
	 */
	virtual CompanyNode* clone();

	/** @brief Destroys the team and its members. */
	~Team();
};

#endif
