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
	/** @brief Starts with no employee under construction. */
	EmployeeBuilder() : employee(nullptr) {}

	/** @brief Drops the current employee and starts a new one. */
	virtual void reset() = 0;

	/**
	 * @brief Adds a competency level; false if the name is not recognised.
	 * @param name Level name ("Intern", "Junior" or "Senior").
	 * @return True if added, false if the level is not recognised.
	 */
	virtual bool addCompetency(std::string name) = 0;

	/**
	 * @brief Adds a profession to the employee being built.
	 * @param name Profession name.
	 * @return True if added.
	 */
	virtual bool addProfession(std::string name) = 0;

	/**
	 * @brief Finishes the employee and hands it over; the caller owns it.
	 * @param name Name for the employee.
	 * @param isManager True to build a Manager.
	 * @return The finished employee; the caller owns it.
	 */
	virtual EmployeeComponent* finish(std::string name, bool isManager = false) = 0;
};

#endif
