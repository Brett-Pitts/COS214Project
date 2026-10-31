#ifndef CONCRETEEMPLOYEEBUILDER_H
#define CONCRETEEMPLOYEEBUILDER_H

#include "EmployeeBuilder.h"

/**
 * @class ConcreteEmployeeBuilder
 * @brief Builds an employee by wrapping it in profession and competency decorators.
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
	/** @brief Discards any employee under construction and starts a new one. */
	void reset();

	/**
	 * @brief Wraps the employee under construction with a competency level.
	 * @param name Name of the competency level (for example "Intern", "Junior" or "Senior").
	 * @return true if the competency was added, false if the name is not recognised.
	 */
	bool addCompetency(std::string name);

	/**
	 * @brief Wraps the employee under construction with a profession.
	 * @param name Name of the profession. It must already be registered.
	 * @return true if the profession was added, false if it is not registered.
	 */
	bool addProfession(std::string name);

	/**
	 * @brief Completes the employee and hands it to the caller.
	 * @param name Name given to the employee.
	 * @param isManager If true the core is a Manager, otherwise an Employee.
	 * @return The finished employee. The caller owns it.
	 */
	EmployeeComponent* finish(std::string name, bool isManager = false);
};

#endif
