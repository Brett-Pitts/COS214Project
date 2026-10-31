#ifndef EMPLOYEEPROFESSION_H
#define EMPLOYEEPROFESSION_H

#include "EmployeeDetail.h"

#include <vector>
class FileSystem;

/**
 * @class EmployeeProfession
 * @brief Decorator that gives an employee a profession (for example coder or plumber).
 *
 * Professions are registered by name at runtime in a static registry, so a new
 * industry needs no new class.
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
	 * @brief Template-method step: prepare the work unit, then pass the step inward.
	 * @param workUnit The unit being worked on.
	 */
	void prepare(WorkUnit* workUnit);

	/**
	 * @brief Template-method step: perform the work, then pass the step inward.
	 * @param workUnit The unit being worked on.
	 */
	void perform(WorkUnit* workUnit);

	/**
	 * @brief Template-method step: finish the work, then pass the step inward.
	 * @param workUnit The unit being worked on.
	 */
	void finish(WorkUnit* workUnit);

public:
	/**
	 * @brief Creates a profession decorator.
	 * @param name Name of the profession. It should be registered first.
	 */
	EmployeeProfession(std::string name);

	/**
	 * @brief Copy constructor.
	 * @param other The decorator to copy.
	 */
	EmployeeProfession(const EmployeeProfession& other);

	/** @brief Virtual destructor. */
	virtual ~EmployeeProfession();

	/** @brief Gets the kind of component ("Profession"). */
	std::string getKind();

	/**
	 * @brief Adds a profession to the registry.
	 * @param name Name of the profession.
	 * @return The registered name.
	 */
	static std::string registerProfession(std::string name);

	/**
	 * @brief Removes a profession from the registry.
	 * @param name Name of the profession.
	 * @return true if it was found and removed.
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
	 * @param professionName Name of the profession to compare.
	 * @return true if it matches this decorator's profession or one further in the chain.
	 */
	bool compareProfession(std::string professionName);

	/**
	 * @brief Makes a copy of this decorator and the components it wraps.
	 * @return A newly allocated copy. The caller owns it.
	 */
	EmployeeComponent* clone();
};

#endif
