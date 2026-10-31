#ifndef EMPLOYEECOMPONENT_H
#define EMPLOYEECOMPONENT_H

#include <string>


using namespace std;


class Mediator;
class WorkUnit;
class FileSystem;

/**
 * @class EmployeeComponent
 * @brief Abstract base of every employee and every employee decorator.
 *
 * Defines the interface the rest of the system sees. It also implements the
 * links that let employees take part in a team: a successor (the next
 * employee in the chain) and a mediator (the team).
 *
 * @par Design patterns
 * - Decorator (Component): Employee and Manager are the concrete components,
 *   EmployeeDetail is the decorator.
 * - Chain of Responsibility (Handler): handleWork() passes a work unit to the
 *   successor when this employee cannot do it.
 * - Template Method (AbstractClass): doWork() runs prepare, perform and finish.
 * - Mediator (Colleague): receive() gets messages from the team.
 * - Prototype (Prototype): clone() copies an employee.
 *
 * @see Employee, EmployeeDetail
 */
class EmployeeComponent {
	friend class FileSystem;
private:
	Mediator* mediator;            ///< The team this employee reports to, or nullptr.
	EmployeeComponent* successor;  ///< Next employee in the chain, or nullptr.

	/**
	 * @brief Checks whether this employee may advance a work unit in its current state.
	 * @param workUnit The unit to check.
	 * @return true if this employee has the right profession and level.
	 */
	bool canAdvance(WorkUnit* workUnit);

protected:
	/**
	 * @brief Template-method step run when the unit is Ready (intern level).
	 * @param workUnit The unit being worked on.
	 */
	virtual void prepare(WorkUnit* workUnit);

	/**
	 * @brief Template-method step run when the unit is Ongoing (junior level).
	 * @param workUnit The unit being worked on.
	 */
	virtual void perform(WorkUnit* workUnit);

	/**
	 * @brief Template-method step run when the unit is Escalated (senior level).
	 * @param workUnit The unit being worked on.
	 */
	virtual void finish(WorkUnit* workUnit);

public:
	/** @brief Creates a component with no mediator and no successor. */
	EmployeeComponent();

	/**
	 * @brief Copy constructor.
	 * @param other The component to copy.
	 */
	EmployeeComponent(const EmployeeComponent& other);

	/** @brief Virtual destructor. */
	virtual ~EmployeeComponent();

	/**
	 * @brief Gets the innermost (undecorated) employee.
	 * @return The core Employee or Manager at the bottom of the decorator chain.
	 */
	virtual EmployeeComponent* getCore();

	/** @brief Gets the kind of component, for example "Employee" or "Profession". */
	virtual std::string getKind() = 0;

	/** @brief Gets the employee's name. */
	virtual string getName() = 0;

	/**
	 * @brief Checks whether this employee has a given profession.
	 * @param professionName Name of the profession.
	 * @return true if the profession matches.
	 */
	virtual bool compareProfession(string professionName);

	/**
	 * @brief Checks whether this employee has the required competency level.
	 * @param requiredCompetency The level required (see EmployeeCompetency).
	 * @return true if the level matches.
	 */
	virtual bool compareCompetency(int requiredCompetency);

	/**
	 * @brief Does one step of work on a unit, based on the unit's current state.
	 * @param workUnit The unit being worked on.
	 */
	virtual void doWork(WorkUnit* workUnit);

	/**
	 * @brief Handles a work unit, or passes it along the chain.
	 * @param workUnit The unit to handle.
	 * @return true if this employee or a later one in the chain did the work.
	 */
	bool handleWork(WorkUnit* workUnit);

	/**
	 * @brief Receives a message from the team mediator.
	 * @param message The message text.
	 */
	virtual void receive(string message);

	/**
	 * @brief Sets the next employee in the chain of responsibility.
	 * @param employeeComponent The successor.
	 */
	void setSuccessor(EmployeeComponent* employeeComponent);

	/**
	 * @brief Sets the mediator (team) this employee reports to.
	 * @param mediator The mediator.
	 */
	virtual void setMediator(Mediator* mediator);

	/**
	 * @brief Wraps another component inside this one.
	 * @param component The component to wrap.
	 */
	virtual void wrap(EmployeeComponent* component) = 0;

	/**
	 * @brief Makes a deep copy of this component.
	 * @return A newly allocated copy. The caller owns it.
	 */
	virtual EmployeeComponent* clone() = 0;
};

#endif
