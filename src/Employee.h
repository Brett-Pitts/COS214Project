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

	/**
	 * @brief Does one step of work on a work unit.
	 * @param workUnit The unit being worked on.
	 */
	void doWork(WorkUnit* workUnit);

	/**
	 * @brief Not supported on the core: an Employee cannot wrap another component.
	 * @param component The component that would be wrapped.
	 */
	void wrap(EmployeeComponent* component);

	/**
	 * @brief Makes a copy of this employee.
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
