#ifndef EMPLOYEECOMPONENT_H
#define EMPLOYEECOMPONENT_H

#include <string>

#include "CompetencyLevel.h"
using std::string;


class Mediator;
class WorkUnit;
class FileSystem;

class EmployeeComponent {
	friend class FileSystem;
private:
	Mediator* mediator;
	EmployeeComponent* successor;

	/**
	 * @brief Checks if the employee's competency is enough for the unit's current state.
	 * @param workUnit The unit being worked on.
	 * @return True if the employee can advance the unit.
	 */
	bool canAdvance(WorkUnit* workUnit);

protected:
	/**
	 * @brief Template step: prints that the employee prepares the unit.
	 * @param workUnit The unit being worked on.
	 */
	virtual void prepare(WorkUnit* workUnit);

	/**
	 * @brief Template step: prints that the employee performs the unit.
	 * @param workUnit The unit being worked on.
	 */
	virtual void perform(WorkUnit* workUnit);

	/**
	 * @brief Template step: prints that the employee finishes the unit.
	 * @param workUnit The unit being worked on.
	 */
	virtual void finish(WorkUnit* workUnit);

public:
	/** @brief Creates a component with no mediator or successor. */
	EmployeeComponent();

	/**
	 * @brief Copies the mediator and successor pointers.
	 * @param other The one to copy.
	 */
	EmployeeComponent(const EmployeeComponent& other);

	/** @brief Virtual destructor. */
	virtual ~EmployeeComponent();

	/**
	 * @brief Gets the innermost component (this one by default).
	 * @return The innermost component.
	 */
	virtual EmployeeComponent* getCore();

	/**
	 * @brief Gets the kind of component, e.g. Employee or Profession.
	 * @return The kind of component.
	 */
	virtual std::string getKind() = 0;

	/**
	 * @brief Gets the employee's name.
	 * @return The name.
	 */
	virtual string getName() = 0;

	/**
	 * @brief Checks for a profession; false by default.
	 * @param professionName Profession to check.
	 * @return True if the profession matches.
	 */
	virtual bool compareProfession(string professionName);

	/**
	 * @brief Checks for a competency level; false by default.
	 * @param requiredCompetency The level required.
	 * @return True if the level is met.
	 */
	virtual bool compareCompetency(CompetencyLevel requiredCompetency);

	/**
	 * @brief Gets the profession name; empty by default.
	 * @return The profession name.
	 */
	virtual std::string getProfession();

	/**
	 * @brief Gets the competency name; empty by default.
	 * @return The competency name.
	 */
	virtual std::string getCompetency();

	/**
	 * @brief Runs prepare, perform and finish, then tells the mediator.
	 * @param workUnit The unit being worked on.
	 */
	virtual void doWork(WorkUnit* workUnit);

	/**
	 * @brief Advances the unit if this employee can, otherwise passes it to the successor.
	 * @param workUnit The unit being worked on.
	 * @return True if someone advanced the unit.
	 */
	bool handleWork(WorkUnit* workUnit);

	/**
	 * @brief Prints the message this employee received.
	 * @param message The message text.
	 */
	virtual void receive(string message);

	/**
	 * @brief Sets the next employee in the chain.
	 * @param employeeComponent The next employee in the chain.
	 */
	void setSuccessor(EmployeeComponent* employeeComponent);

	/**
	 * @brief Sets the mediator (the team).
	 * @param mediator The mediator.
	 */
	virtual void setMediator(Mediator* mediator);

	/**
	 * @brief Wraps another component inside this one.
	 * @param component The component to wrap.
	 */
	virtual void wrap(EmployeeComponent* component) = 0;

	/**
	 * @brief Makes a copy of this component.
	 * @return A new copy; the caller owns it.
	 */
	virtual EmployeeComponent* clone() = 0;
};

#endif
