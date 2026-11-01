#ifndef EMPLOYEEPROFESSION_H
#define EMPLOYEEPROFESSION_H

#include "EmployeeDetail.h"

#include <vector>
class FileSystem;

/**
 * @class EmployeeProfession
 * @brief Decorator that gives an employee a profession (for example coder or plumber).
 *
 * Professions are kept by name in a static registry, so a new industry needs no
 * new class. The constructor adds the name to the registry if it is not there yet.
 *
 * @par Design patterns
 * - Decorator (ConcreteDecorator).
 * - Template Method (ConcreteClass): adds its part to prepare, perform and finish.
 * - Prototype (ConcretePrototype): clone() copies the decorator chain.
 *
 * @see EmployeeDetail, EmployeeCompetency
 */
class EmployeeProfession : public EmployeeDetail {
	friend class FileSystem;
private:
	std::string professionName;                  ///< Name of this employee's profession.
	static std::vector<std::string> professionList; ///< Registry of all valid professions.

protected:
	/**
	 * @brief Prints that the employee reviews the unit as their profession.
	 * @param workUnit The unit being worked on.
	 */
	void prepare(WorkUnit* workUnit);

	/**
	 * @brief Prints that the employee applies their profession's expertise.
	 * @param workUnit The unit being worked on.
	 */
	void perform(WorkUnit* workUnit);

	/**
	 * @brief Prints that the employee signs off the unit.
	 * @param workUnit The unit being worked on.
	 */
	void finish(WorkUnit* workUnit);

public:
	/**
	 * @brief Creates a decorator and registers the profession.
	 * @param name Profession name.
	 */
	EmployeeProfession(std::string name);

	/**
	 * @brief Gets the profession name.
	 * @return The profession name.
	 */
	virtual std::string getProfession();

	/**
	 * @brief Copies the profession and deep-copies the wrapped component.
	 * @param other The decorator to copy.
	 */
	EmployeeProfession(const EmployeeProfession& other);

	/** @brief Virtual destructor. */
	virtual ~EmployeeProfession();

	/**
	 * @brief Returns "Profession".
	 * @return The kind of component.
	 */
	std::string getKind();

	/**
	 * @brief Adds a profession to the registry, ignoring empty or duplicate names.
	 * @param name Profession name.
	 * @return The name, or "" if it was empty.
	 */
	static std::string registerProfession(std::string name);

	/**
	 * @brief Removes a profession from the registry; false if it isn't there.
	 * @param name Profession name.
	 * @return False if it isn't registered.
	 */
	static bool deleteProfession(std::string name);

	/**
	 * @brief Checks whether a profession is registered.
	 * @param name Profession name.
	 * @return True if registered.
	 */
	static bool hasProfession(std::string name);

	/**
	 * @brief True if this decorator, or one further in, has the profession.
	 * @param professionName Profession to check.
	 * @return True if the profession matches.
	 */
	bool compareProfession(std::string professionName);

	/**
	 * @brief Copies this decorator and the chain it wraps.
	 * @return A new copy; the caller owns it.
	 */
	EmployeeComponent* clone();
};

#endif
