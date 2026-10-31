#ifndef EMPLOYEEDIRECTOR_H
#define EMPLOYEEDIRECTOR_H
 
#include <string>
#include "EmployeeComponent.h"
#include "EmployeeBuilder.h"

/**
 * @class EmployeeDirector
 * @brief Runs the build steps in the right order to create employees of a given level.
 *
 * @par Design pattern
 * Builder (Director). It uses an EmployeeBuilder, so the same steps can build
 * different representations.
 *
 * @see EmployeeBuilder, ConcreteEmployeeBuilder
 */
class EmployeeDirector {
private:
	EmployeeBuilder* builder; ///< The builder used to create employees. Not owned.

	/**
	 * @brief Runs reset, addProfession, addCompetency and finish on the builder.
	 *
	 * The results of addProfession() and addCompetency() are not checked, so the
	 * function does not return nullptr: if the competency name is not recognised, the
	 * employee is built without a competency decorator. The employee is never a Manager.
	 *
	 * @param name Name of the employee.
	 * @param profession Name of the profession.
	 * @param competency Name of the competency level, passed to the builder as it is.
	 * @return The finished employee. The caller owns it.
	 */
	EmployeeComponent* build(std::string name, std::string profession, std::string competency);
 
public:

	/**
	 * @brief Creates a director.
	 * @param builder The builder to use. The director does not take ownership. It must not be null (this is not checked).
	 */
	EmployeeDirector(EmployeeBuilder* builder);

	/**
	 * @brief Changes the builder.
	 * @param builder The new builder. The director does not take ownership. If it is null, a message is printed to the error stream and the old builder is kept.
	 */
	void setBuilder(EmployeeBuilder* builder);

	/**
	 * @brief Builds an employee with the competency "intern" (see build()).
	 *
	 * @warning The level is passed to the builder in lower case. EmployeeCompetency
	 * only recognises "Intern", "Junior" and "Senior" (case-sensitive), so
	 * ConcreteEmployeeBuilder rejects it and the employee is built with a profession
	 * but without a competency. Such an employee cannot advance any work unit.
	 *
	 * @param name Name of the employee.
	 * @param profession Name of the profession.
	 * @return The new employee. The caller owns it.
	 */
	EmployeeComponent* makeIntern(std::string name, std::string profession);

	/**
	 * @brief Builds an employee with the competency "junior" (see build()).
	 *
	 * @warning The level is passed to the builder in lower case. EmployeeCompetency
	 * only recognises "Intern", "Junior" and "Senior" (case-sensitive), so
	 * ConcreteEmployeeBuilder rejects it and the employee is built with a profession
	 * but without a competency. Such an employee cannot advance any work unit.
	 *
	 * @param name Name of the employee.
	 * @param profession Name of the profession.
	 * @return The new employee. The caller owns it.
	 */
	EmployeeComponent* makeJunior(std::string name, std::string profession);

	/**
	 * @brief Builds an employee with the competency "senior" (see build()).
	 *
	 * @warning The level is passed to the builder in lower case. EmployeeCompetency
	 * only recognises "Intern", "Junior" and "Senior" (case-sensitive), so
	 * ConcreteEmployeeBuilder rejects it and the employee is built with a profession
	 * but without a competency. Such an employee cannot advance any work unit.
	 *
	 * @param name Name of the employee.
	 * @param profession Name of the profession.
	 * @return The new employee. The caller owns it.
	 */
	EmployeeComponent* makeSenior(std::string name, std::string profession);
};
 
#endif
