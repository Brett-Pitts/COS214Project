#ifndef CONCRETEEMPLOYEEBUILDER_H
#define CONCRETEEMPLOYEEBUILDER_H

#include "EmployeeBuilder.h"

/**
 * @class ConcreteEmployeeBuilder
 * @brief Builds an employee by wrapping it in profession and competency decorators.
 *
 * Parts are added from the outside in: the first part that is added becomes the
 * outermost decorator, each later part is placed inside the earlier ones, and
 * finish() puts the Employee or Manager core in the middle.
 *
 * @par Design pattern
 * Builder (ConcreteBuilder). The product is an EmployeeComponent made of an
 * Employee or Manager core wrapped by EmployeeProfession and
 * EmployeeCompetency decorators.
 *
 * @see EmployeeBuilder, EmployeeDirector, EmployeeProfession, EmployeeCompetency
 */
class ConcreteEmployeeBuilder : public EmployeeBuilder {


public:
	/**
	 * @brief Deletes any employee under construction, so that the next part added starts a new one.
	 */
	void reset();

	/**
	 * @brief Adds a competency decorator to the employee under construction.
	 * @param name Name of the level: "Intern", "Junior" or "Senior" (case-sensitive).
	 * @return true if the competency was added, false (and nothing is added) if the name is not recognised.
	 */
	bool addCompetency(std::string name);

	/**
	 * @brief Adds a profession decorator to the employee under construction.
	 *
	 * The profession does not have to be registered first: creating the decorator
	 * registers it.
	 *
	 * @param name Name of the profession.
	 * @return Always true.
	 */
	bool addProfession(std::string name);

	/**
	 * @brief Completes the employee and hands it to the caller.
	 *
	 * The core is created last and placed at the centre of the decorators that were
	 * added. If no parts were added, the result is just the core. Afterwards the
	 * builder holds no employee.
	 *
	 * @param name Name given to the employee.
	 * @param isManager If true the core is a Manager, otherwise an Employee.
	 * @return The finished employee. The caller owns it.
	 */
	EmployeeComponent* finish(std::string name, bool isManager = false);
};

#endif
