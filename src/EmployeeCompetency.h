#ifndef EMPLOYEECOMPETENCY_H
#define EMPLOYEECOMPETENCY_H

#include "EmployeeDetail.h"
#include "CompetencyLevel.h"
class FileSystem;

/**
 * @class EmployeeCompetency
 * @brief Decorator that gives an employee a competency level.
 *
 * The levels are ordered Intern, Junior, Senior, and an employee can do
 * everything at or below their own level. The level needed depends on the state
 * of the work unit: Ready needs at least Intern, Ongoing at least Junior and
 * Escalated at least Senior (see EmployeeComponent::canAdvance()). A senior can
 * therefore advance a unit in any of these states, but an intern only in Ready.
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
	CompetencyLevel competencyId;        ///< Numeric level: INTERN, JUNIOR or SENIOR, or -1 if the name was not recognised.

protected:
	/**
	 * @brief Template-method step: prints that the employee gathers tools of this level for the unit.
	 * @param workUnit The unit being worked on.
	 */
	void prepare(WorkUnit* workUnit);

	/**
	 * @brief Template-method step: prints that the employee uses skills of this level on the unit.
	 * @param workUnit The unit being worked on.
	 */
	void perform(WorkUnit* workUnit);

	/**
	 * @brief Template-method step: prints that the employee records the outcome for the unit.
	 * @param workUnit The unit being worked on.
	 */
	void finish(WorkUnit* workUnit);

public:
	/** @brief Competency levels, in increasing order of seniority. */


	/**
	 * @brief Creates a competency decorator.
	 * @param name Name of the level: "Intern", "Junior" or "Senior" (case-sensitive).
	 *        Any other name gives the level -1, which never satisfies a requirement.
	 */
	EmployeeCompetency(CompetencyLevel level);

	/**
	 * @brief Copy constructor. Copies the level and makes a deep copy of the wrapped component.
	 * @param other The decorator to copy.
	 */
	EmployeeCompetency(const EmployeeCompetency& other);
	virtual std::string getCompetency();

	/** @brief Virtual destructor. */
	virtual ~EmployeeCompetency();

	/** @brief Gets the kind of component ("Competency"). */
	std::string getKind();

	/**
	 * @brief Converts a level name to its numeric level.
	 * @param name Name of the level: "Intern", "Junior" or "Senior" (case-sensitive).
	 * @return INTERN, JUNIOR or SENIOR, or -1 if the name is not recognised.
	 */
	static CompetencyLevel getCompetencyLevel(const std::string& name);


	static std::string getCompetencyName(CompetencyLevel level);

	/**
	 * @brief Checks whether this employee is competent enough.
	 * @param requiredCompetency The level required (INTERN, JUNIOR or SENIOR).
	 * @return true if this decorator's level is at least the required level.
	 *         Otherwise the wrapped chain is asked. A negative requirement is never met here.
	 */
	bool compareCompetency(CompetencyLevel requiredCompetency);

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
