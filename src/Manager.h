#ifndef MANAGER_H
#define MANAGER_H

#include "Employee.h"

class Project;

/**
 * @class Manager
 * @brief An employee who can handle escalations and cancel projects.
 *
 * The manager is the last link of a team's chain of responsibility.
 *
 * @par Design patterns
 * - Decorator (ConcreteComponent), as a subclass of Employee.
 * - Chain of Responsibility (ConcreteHandler): the last link in the chain.
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
	 * @brief Handles a work unit that nobody else in the chain could take.
	 * @param workUnit The unit that needs a decision.
	 */
	void handleEscalation(WorkUnit* workUnit);

	/**
	 * @brief Cancels a project and everything in it.
	 * @param project The project to cancel. Each node below it is set to Cancelled.
	 */
	void cancelProject(Project* project);

	/**
	 * @brief Makes a copy of this manager.
	 * @return A newly allocated copy. The caller owns it.
	 */
	virtual EmployeeComponent* clone();
};

#endif
