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
	 * @brief Creates a manager.
	 * @param name The manager's name.
	 */
	Manager(std::string name);

	/**
	 * @brief Copy constructor.
	 * @param other The manager to copy.
	 */
	Manager(const Manager& other);

	/** @brief Virtual destructor. */
	virtual ~Manager();

	/** @brief Gets the kind of component ("Manager"). */
	std::string getKind();

	/**
	 * @brief Handles an escalated work unit. It only prints a line saying that the manager handles the escalation; the unit is not changed.
	 * @param workUnit The unit that needs a decision. A null unit is ignored.
	 */
	void handleEscalation(WorkUnit* workUnit);

	/**
	 * @brief Cancels a project.
	 *
	 * Sets the state of the given project to Cancelled and prints a line saying that
	 * the manager cancels the project. The nodes below the project are not changed.
	 *
	 * @param project The project to cancel. A null project is ignored.
	 */
	void cancelProject(Project* project);

	/**
	 * @brief Makes a copy of this manager with the same name.
	 *
	 * The copy has no mediator and no successor.
	 *
	 * @return A newly allocated copy. The caller owns it.
	 */
	virtual EmployeeComponent* clone();
};

#endif
