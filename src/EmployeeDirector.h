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
	 * @brief Runs the builder steps in order for a name, profession and competency.
	 * @param name Employee name.
	 * @param profession Profession name.
	 * @param competency Competency level name.
	 * @return The new employee; the caller owns it.
	 */
	EmployeeComponent* build(std::string name, std::string profession, std::string competency);
 
public:

	/**
	 * @brief Creates a director that uses the given builder.
	 * @param builder The builder to use (not owned).
	 */
	EmployeeDirector(EmployeeBuilder* builder);

	/**
	 * @brief Swaps the builder; a null one is rejected with a message.
	 * @param builder The new builder (not owned, null is rejected).
	 */
	void setBuilder(EmployeeBuilder* builder);

	/**
	 * @brief Builds an intern (the level is passed in lower case, so it may get no competency).
	 * @param name Employee name.
	 * @param profession Profession name.
	 * @return The new employee; the caller owns it.
	 */
	EmployeeComponent* makeIntern(std::string name, std::string profession);

	/**
	 * @brief Builds a junior (the level is passed in lower case, so it may get no competency).
	 * @param name Employee name.
	 * @param profession Profession name.
	 * @return The new employee; the caller owns it.
	 */
	EmployeeComponent* makeJunior(std::string name, std::string profession);

	/**
	 * @brief Builds a senior (the level is passed in lower case, so it may get no competency).
	 * @param name Employee name.
	 * @param profession Profession name.
	 * @return The new employee; the caller owns it.
	 */
	EmployeeComponent* makeSenior(std::string name, std::string profession);
};
 
#endif
