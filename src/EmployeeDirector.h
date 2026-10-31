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
	 * @param name Name of the employee.
	 * @param profession Name of the profession.
	 * @param competency Name of the competency level.
	 * @return The finished employee, or nullptr if the profession or level is invalid.
	 */
	EmployeeComponent* build(std::string name, std::string profession, std::string competency);
 
public:

	/**
	 * @brief Creates a director.
	 * @param builder The builder to use. The director does not take ownership.
	 */
	EmployeeDirector(EmployeeBuilder* builder);

	/**
	 * @brief Changes the builder.
	 * @param builder The new builder. The director does not take ownership.
	 */
	void setBuilder(EmployeeBuilder* builder);

	/**
	 * @brief Builds an intern.
	 * @param name Name of the employee.
	 * @param profession Name of the profession.
	 * @return The new employee, or nullptr on failure. The caller owns it.
	 */
	EmployeeComponent* makeIntern(std::string name, std::string profession);

	/**
	 * @brief Builds a junior.
	 * @param name Name of the employee.
	 * @param profession Name of the profession.
	 * @return The new employee, or nullptr on failure. The caller owns it.
	 */
	EmployeeComponent* makeJunior(std::string name, std::string profession);

	/**
	 * @brief Builds a senior.
	 * @param name Name of the employee.
	 * @param profession Name of the profession.
	 * @return The new employee, or nullptr on failure. The caller owns it.
	 */
	EmployeeComponent* makeSenior(std::string name, std::string profession);
};
 
#endif
