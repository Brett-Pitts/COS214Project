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
 * employee in the chain) and a mediator (the team). Neither is owned.
 *
 * @par Design patterns
 * - Decorator (Component): Employee and Manager are the concrete components,
 *   EmployeeDetail is the decorator.
 * - Chain of Responsibility (Handler): handleWork() passes a work unit to the
 *   successor when this employee cannot advance it.
 * - Template Method (AbstractClass): doWork() runs prepare, perform and finish.
 * - Mediator (Colleague): receive() gets messages from the team.
 * - Prototype (Prototype): clone() copies an employee.
 *
 * @see Employee, EmployeeDetail
 */
class EmployeeComponent {
	friend class FileSystem;
private:
	Mediator* mediator;            ///< The team this employee reports to, or nullptr. Not owned.
	EmployeeComponent* successor;  ///< Next employee in the chain, or nullptr. Not owned (the team owns its members).

	/**
	 * @brief Checks whether this employee may advance a work unit from its current state.
	 *
	 * The level needed depends on the state of the unit: Ready needs at least Intern,
	 * Ongoing at least Junior and Escalated at least Senior (see
	 * EmployeeCompetency). In any other state the answer is false. Only the level is
	 * checked here; the profession is matched by the Team.
	 *
	 * @param workUnit The unit to check.
	 * @return true if compareCompetency() accepts the level needed for the unit's state.
	 */
	bool canAdvance(WorkUnit* workUnit);

protected:
	/**
	 * @brief Template-method step 1 of doWork(). The base version prints that the employee prepares the unit.
	 * @param workUnit The unit being worked on.
	 */
	virtual void prepare(WorkUnit* workUnit);

	/**
	 * @brief Template-method step 2 of doWork(). The base version prints that the employee performs the unit.
	 * @param workUnit The unit being worked on.
	 */
	virtual void perform(WorkUnit* workUnit);

	/**
	 * @brief Template-method step 3 of doWork(). The base version prints that the employee finishes the unit.
	 * @param workUnit The unit being worked on.
	 */
	virtual void finish(WorkUnit* workUnit);

public:
	/** @brief Creates a component with no mediator and no successor. */
	EmployeeComponent();

	/**
	 * @brief Copy constructor. Copies the mediator and successor pointers.
	 * @param other The component to copy.
	 */
	EmployeeComponent(const EmployeeComponent& other);

	/** @brief Virtual destructor. */
	virtual ~EmployeeComponent();

	/**
	 * @brief Gets the innermost (undecorated) employee.
	 * @return The core Employee or Manager at the bottom of the decorator chain. The base version returns this object.
	 */
	virtual EmployeeComponent* getCore();

	/** @brief Gets the kind of component, for example "Employee" or "Profession". */
	virtual std::string getKind() = 0;

	/** @brief Gets the employee's name. */
	virtual string getName() = 0;

	/**
	 * @brief Checks whether this employee has a given profession.
	 * @param professionName Name of the profession.
	 * @return true if the profession matches. The base version always returns false; only EmployeeProfession answers true.
	 */
	virtual bool compareProfession(string professionName);

	/**
	 * @brief Checks whether this employee has at least the required competency level.
	 * @param requiredCompetency The level required (see EmployeeCompetency).
	 * @return true if the level is high enough. The base version always returns false; only EmployeeCompetency answers true.
	 */
	virtual bool compareCompetency(int requiredCompetency);

	/**
	 * @brief Does the work on a unit by running prepare(), perform() and finish() in that order.
	 *
	 * All three steps are run on every call, whatever the state of the unit. Then the
	 * mediator (if set) is told, so the rest of the team receives a message that this
	 * employee worked on the unit. A null unit is ignored. This does not change the
	 * state of the unit; handleWork() does that afterwards.
	 *
	 * @param workUnit The unit being worked on.
	 */
	virtual void doWork(WorkUnit* workUnit);

	/**
	 * @brief Handles a work unit, or passes it along the chain.
	 *
	 * If this employee can advance the unit (canAdvance()), it does the work with
	 * doWork() and then calls WorkUnit::advanceState(), which moves the unit on by
	 * one state. Otherwise the unit is passed to the successor. If there is no
	 * successor, a message that nobody in the chain can handle the unit is printed.
	 *
	 * @param workUnit The unit to handle. A null unit is not handled.
	 * @return true if this employee or a later one in the chain did the work.
	 */
	bool handleWork(WorkUnit* workUnit);

	/**
	 * @brief Receives a message from the team mediator. The base version prints it with the employee's name.
	 * @param message The message text.
	 */
	virtual void receive(string message);

	/**
	 * @brief Sets the next employee in the chain of responsibility.
	 * @param employeeComponent The successor. Not owned.
	 */
	void setSuccessor(EmployeeComponent* employeeComponent);

	/**
	 * @brief Sets the mediator (team) this employee reports to.
	 * @param mediator The mediator. Not owned.
	 */
	virtual void setMediator(Mediator* mediator);

	/**
	 * @brief Wraps another component inside this one.
	 *
	 * See EmployeeDetail::wrap(), which adds the component to the decorator chain,
	 * and Employee::wrap(), which cannot wrap anything.
	 *
	 * @param component The component to wrap.
	 */
	virtual void wrap(EmployeeComponent* component) = 0;

	/**
	 * @brief Makes a deep copy of this component, including everything it wraps.
	 *
	 * The copy has no mediator and no successor.
	 *
	 * @return A newly allocated copy. The caller owns it.
	 */
	virtual EmployeeComponent* clone() = 0;
};

#endif
