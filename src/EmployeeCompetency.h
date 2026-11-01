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
	 * @brief Prints that the employee gets tools of this level ready.
	 * @param workUnit The unit being worked on.
	 */
	void prepare(WorkUnit* workUnit);

	/**
	 * @brief Prints that the employee uses skills of this level on the unit.
	 * @param workUnit The unit being worked on.
	 */
	void perform(WorkUnit* workUnit);

	/**
	 * @brief Prints that the employee records the outcome.
	 * @param workUnit The unit being worked on.
	 */
	void finish(WorkUnit* workUnit);

public:
	/**
	 * @brief Creates a decorator with the given competency level.
	 * @param level The competency level.
	 */
	EmployeeCompetency(CompetencyLevel level);

	/**
	 * @brief Copies the level and deep-copies the wrapped component.
	 * @param other The decorator to copy.
	 */
	EmployeeCompetency(const EmployeeCompetency& other);

	/**
	 * @brief Gets the name of this level.
	 * @return The competency name.
	 */
	virtual std::string getCompetency();

	/** @brief Virtual destructor. */
	virtual ~EmployeeCompetency();

	/**
	 * @brief Returns "Competency".
	 * @return The kind of component.
	 */
	std::string getKind();

	/**
	 * @brief Turns a level name into a CompetencyLevel (NONE if unknown).
	 * @param name Level name.
	 * @return The matching level, or NONE.
	 */
	static CompetencyLevel getCompetencyLevel(const std::string& name);


	/**
	 * @brief Turns a CompetencyLevel into its name.
	 * @param level The level.
	 * @return The level's name.
	 */
	static std::string getCompetencyName(CompetencyLevel level);

	/**
	 * @brief True if this level, or one further in, is at least the required one.
	 * @param requiredCompetency The level required.
	 * @return True if the level is met.
	 */
	bool compareCompetency(CompetencyLevel requiredCompetency);

	/**
	 * @brief Copies this decorator and the chain it wraps.
	 * @return A new copy; the caller owns it.
	 */
	EmployeeComponent* clone();
};

#endif
