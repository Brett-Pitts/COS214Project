#ifndef MANAGER_H
#define MANAGER_H

#include "Employee.h"

class Project;

/**
 * @class Manager
 * @brief An employee with two extra operations: handling escalations and cancelling projects.
 *
 * A Manager takes part in the chain of responsibility like any other employee, in
 * the position in which it was added to the team.
 *
 * @par Design patterns
 * - Decorator (ConcreteComponent), as a subclass of Employee.
 * - Chain of Responsibility (ConcreteHandler): handles work like any other member.
 * - Prototype (ConcretePrototype): clone() copies the manager.
 *
 * @see Employee, Project, Cancelled
 */
class Manager : public Employee {

public:
	/**
	 * @brief Creates a manager with the given name.
	 * @param name Manager name.
	 */
	Manager(std::string name);

	/**
	 * @brief Copies another manager.
	 * @param other The one to copy.
	 */
	Manager(const Manager& other);

	/** @brief Virtual destructor. */
	virtual ~Manager();

	/**
	 * @brief Always false, same as Employee.
	 * @param requiredCompetency The level required.
	 * @return Always false.
	 */
	virtual bool compareCompetency(int requiredCompetency);

	/**
	 * @brief Returns "Manager".
	 * @return The kind of component.
	 */
	std::string getKind();

	/**
	 * @brief Prints that the manager handles the escalation; the unit is untouched.
	 * @param workUnit The escalated unit (null is ignored).
	 */
	void handleEscalation(WorkUnit* workUnit);

	/**
	 * @brief Sets the project to Cancelled and prints a message.
	 * @param project The project to cancel (null is ignored).
	 */
	void cancelProject(Project* project);

	/**
	 * @brief Copies the manager (same name, no mediator or successor).
	 * @return A new copy; the caller owns it.
	 */
	virtual EmployeeComponent* clone();
};

#endif
