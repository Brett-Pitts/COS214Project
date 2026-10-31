#ifndef EMPLOYEEBUILDER_H
#define EMPLOYEEBUILDER_H

#include "EmployeeComponent.h"
#include <string>

/**
 * @class EmployeeBuilder
 * @brief Abstract builder that assembles an employee from optional parts.
 *
 * @par Design pattern
 * Builder (Builder). The product is an EmployeeComponent. Use
 * EmployeeDirector to run the build steps in the right order.
 *
 * @see ConcreteEmployeeBuilder, EmployeeDirector
 */
class EmployeeBuilder {

protected:
	EmployeeComponent* employee; ///< The employee under construction, or nullptr.

public:
	/** @brief Creates a builder with no employee under construction. */
	EmployeeBuilder() : employee(nullptr) {}

	/** @brief Discards the current employee and starts a new one. */
	virtual void reset() = 0;

	/**
	 * @brief Adds a competency level to the employee under construction.
	 * @param name Name of the competency level.
	 * @return true on success, false if the level is not recognised.
	 */
	virtual bool addCompetency(std::string name) = 0;

	/**
	 * @brief Adds a profession to the employee under construction.
	 * @param name Name of the profession.
	 * @return true on success, false if the profession is not registered.
	 */
	virtual bool addProfession(std::string name) = 0;

	/**
	 * @brief Completes the employee and returns it.
	 * @param name Name given to the employee.
	 * @param isManager If true, a Manager is built instead of an Employee.
	 * @return The finished employee. The caller owns it.
	 */
	virtual EmployeeComponent* finish(std::string name, bool isManager = false) = 0;
};

#endif
