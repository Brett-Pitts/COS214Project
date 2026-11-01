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
	 * @brief Creates an employee with the given name.
	 * @param name Employee name.
	 */
	Employee(std::string name);

	/**
	 * @brief Copies another employee.
	 * @param other The one to copy.
	 */
	Employee(const Employee& other);

	/** @brief Virtual destructor. */
	virtual ~Employee();

	/**
	 * @brief Gets the employee's name.
	 * @return The name.
	 */
	std::string getName();

	/**
	 * @brief Returns "Employee".
	 * @return The kind of component.
	 */
	std::string getKind();

	/**
	 * @brief Always false, competency comes from the decorators.
	 * @param requiredCompetency The level required.
	 * @return Always false.
	 */
	virtual bool compareCompetency(int requiredCompetency);

	/**
	 * @brief Does the prepare, perform and finish steps, then tells the mediator.
	 * @param workUnit The unit being worked on.
	 */
	void doWork(WorkUnit* workUnit);

	/**
	 * @brief Not supported, so it just deletes the component it was given.
	 * @param component The component to wrap (it gets deleted).
	 */
	void wrap(EmployeeComponent* component);

	/**
	 * @brief Copies the employee (same name, no mediator or successor).
	 * @return A new copy; the caller owns it.
	 */
	virtual EmployeeComponent* clone();

	/**
	 * @brief Always false, professions come from the decorators.
	 * @param professionName Profession to check.
	 * @return Always false.
	 */
	bool compareProfession(std::string professionName);
};

#endif
