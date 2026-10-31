#ifndef EMPLOYEECOMPETENCY_H
#define EMPLOYEECOMPETENCY_H

#include "EmployeeDetail.h"

class FileSystem;

/**
 * @class EmployeeCompetency
 * @brief Decorator that gives an employee a competency level.
 *
 * The level (intern, junior or senior) decides which step of the work the
 * employee may do: interns prepare, juniors perform and seniors finish.
 *
 * @par Design patterns
 * - Decorator (ConcreteDecorator).
 * - Template Method (ConcreteClass): adds its part to prepare, perform and finish.
 * - Prototype (ConcretePrototype): clone() copies the decorator chain.
 *
 * @see EmployeeDetail, EmployeeProfession
 */
class EmployeeCompetency : public EmployeeDetail {
	friend class FileSystem;
private:
	int competencyId;        ///< Numeric level: INTERN, JUNIOR or SENIOR.
	string competencyName;   ///< Name of the level as entered by the user.

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
	/** @brief Competency levels, in increasing order of seniority. */
	enum { INTERN = 0, JUNIOR = 1, SENIOR = 2 };

	/**
	 * @brief Creates a competency decorator.
	 * @param name Name of the level (for example "Intern").
	 */
	EmployeeCompetency(string name);

	/**
	 * @brief Copy constructor.
	 * @param other The decorator to copy.
	 */
	EmployeeCompetency(const EmployeeCompetency& other);

	/** @brief Virtual destructor. */
	virtual ~EmployeeCompetency();

	/** @brief Gets the kind of component ("Competency"). */
	std::string getKind();

	/**
	 * @brief Converts a level name to its numeric level.
	 * @param name Name of the level.
	 * @return INTERN, JUNIOR or SENIOR, or a negative value if the name is not recognised.
	 */
	static int getCompetencyLevel(string name);

	/**
	 * @brief Checks whether this employee is competent enough.
	 * @param requiredCompetency The level required (INTERN, JUNIOR or SENIOR).
	 * @return true if this employee's level matches the requirement.
	 */
	bool compareCompetency(int requiredCompetency);

	/**
	 * @brief Makes a copy of this decorator and the components it wraps.
	 * @return A newly allocated copy. The caller owns it.
	 */
	EmployeeComponent* clone();
};

#endif
