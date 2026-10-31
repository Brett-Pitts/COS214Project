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
	 * @brief Template-method step: prints that the employee reviews the unit as a member of this profession.
	 * @param workUnit The unit being worked on.
	 */
	void prepare(WorkUnit* workUnit);

	/**
	 * @brief Template-method step: prints that the employee applies the profession's expertise to the unit.
	 * @param workUnit The unit being worked on.
	 */
	void perform(WorkUnit* workUnit);

	/**
	 * @brief Template-method step: prints that the employee signs off the unit as a member of this profession.
	 * @param workUnit The unit being worked on.
	 */
	void finish(WorkUnit* workUnit);

public:
	/**
	 * @brief Creates a profession decorator and registers the profession.
	 *
	 * The name is added to the registry if it is not there yet (an empty name is not
	 * added).
	 *
	 * @param name Name of the profession.
	 */
	EmployeeProfession(std::string name);

	virtual std::string getProfession();

	/**
	 * @brief Copy constructor. Copies the profession name and makes a deep copy of the wrapped component.
	 * @param other The decorator to copy.
	 */
	EmployeeProfession(const EmployeeProfession& other);

	/** @brief Virtual destructor. */
	virtual ~EmployeeProfession();

	/** @brief Gets the kind of component ("Profession"). */
	std::string getKind();

	/**
	 * @brief Adds a profession to the registry.
	 *
	 * Nothing is added if the name is empty or already registered.
	 *
	 * @param name Name of the profession.
	 * @return The name, or an empty string if the name was empty.
	 */
	static std::string registerProfession(std::string name);

	/**
	 * @brief Removes a profession from the registry.
	 *
	 * @param name Name of the profession.
	 * @return false if the profession is not registered.
	 *
	 * @warning The function has no return statement after it removes a profession, so
	 * the value returned in that case is undefined.
	 */
	static bool deleteProfession(std::string name);

	/**
	 * @brief Checks whether a profession is registered.
	 * @param name Name of the profession.
	 * @return true if it is in the registry.
	 */
	static bool hasProfession(std::string name);

	/**
	 * @brief Checks whether this employee has a given profession.
	 *
	 * The decorator must wrap something when the name does not match; this is not
	 * checked.
	 *
	 * @param professionName Name of the profession to compare.
	 * @return true if it matches this decorator's profession or one further in the chain.
	 */
	bool compareProfession(std::string professionName);

	/**
	 * @brief Makes a copy of this decorator, wrapping a copy of the component it wraps.
	 *
	 * The copy has no mediator and no successor.
	 *
	 * @return A newly allocated copy. The caller owns it.
	 */
	EmployeeComponent* clone();
};

#endif
