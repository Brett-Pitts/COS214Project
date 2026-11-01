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
	/** @brief Throws away any half-built employee so the next part starts a fresh one. */
	void reset();

	/**
	 * @brief Adds a competency decorator; false if the level name is not recognised.
	 * @param name Level name ("Intern", "Junior" or "Senior").
	 * @return True if added, false if the level is not recognised.
	 */
	bool addCompetency(std::string name);

	/**
	 * @brief Adds a profession decorator (registering the profession); always true.
	 * @param name Profession name.
	 * @return Always true.
	 */
	bool addProfession(std::string name);

	/**
	 * @brief Puts the Employee or Manager core in the middle and hands over the employee.
	 * @param name Name for the employee.
	 * @param isManager True to build a Manager.
	 * @return The finished employee; the caller owns it.
	 */
	EmployeeComponent* finish(std::string name, bool isManager = false);
};

#endif
