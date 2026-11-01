#ifndef EMPLOYEE_H
#define EMPLOYEE_H

#include "EmployeeComponent.h"

/**
 * @class Employee
 * @brief The core employee: a named person who can be wrapped with decorators.
 *
 * An Employee has no profession or competency on its own. These are added by
 * wrapping it in EmployeeProfession and EmployeeCompetency decorators.
 *
 * @par Design patterns
 * - Decorator (ConcreteComponent).
 * - Template Method (ConcreteClass): supplies the base prepare, perform and
 *   finish steps.
 * - Prototype (ConcretePrototype): clone() copies the employee.
 *
 * @see EmployeeComponent, EmployeeDetail
 */
class Employee : public EmployeeComponent {

private:
	std::string name; ///< The employee's name.

public:
	/**
	 * @brief Creates an employee.
	 * @param name The employee's name.
	 */
	Employee(std::string name);

	/**
	 * @brief Copy constructor.
	 * @param other The employee to copy.
	 */
	Employee(const Employee& other);

	/** @brief Virtual destructor. */
	virtual ~Employee();

	/** @brief Gets the employee's name. */
	std::string getName();

	/** @brief Gets the kind of component ("Employee"). */
	std::string getKind();
	/** @brief Always false, the core has no competency. */
	virtual bool compareCompetency(int requiredCompetency);
	/**
	 * @brief Does the work on a unit.
	 *
	 * Runs the template steps of EmployeeComponent::doWork(): prepare, perform and
	 * finish, and then tells the mediator. For a decorated employee this is the
	 * innermost step of the decorators' doWork().
	 *
	 * @param workUnit The unit being worked on.
	 */
	void doWork(WorkUnit* workUnit);

	/**
	 * @brief Not supported: an Employee is always the innermost component and cannot wrap another.
	 *
	 * The component that is passed in is deleted.
	 *
	 * @param component The component that would be wrapped. It is deleted.
	 */
	void wrap(EmployeeComponent* component);

	/**
	 * @brief Makes a copy of this employee with the same name.
	 *
	 * The copy has no mediator and no successor.
	 *
	 * @return A newly allocated copy. The caller owns it.
	 */
	virtual EmployeeComponent* clone();

	/**
	 * @brief Checks whether this employee has a given profession.
	 * @param professionName Name of the profession to check.
	 * @return Always false for the core, since only decorators carry professions.
	 */
	bool compareProfession(std::string professionName);
};

#endif
