#ifndef EMPLOYEEDETAIL_H
#define EMPLOYEEDETAIL_H

#include "EmployeeComponent.h"
class FileSystem;

/**
 * @class EmployeeDetail
 * @brief Abstract decorator that adds a detail (profession or competency) to an employee.
 *
 * Holds the component it wraps and forwards every operation to it, so that
 * concrete decorators only add their own part.
 *
 * @par Design patterns
 * - Decorator (Decorator).
 * - Template Method: forwards prepare, perform and finish to the wrapped
 *   component.
 *
 * @see EmployeeProfession, EmployeeCompetency
 */
class EmployeeDetail : public EmployeeComponent {
	friend class FileSystem;
private:

protected:
	EmployeeComponent* wrapped; ///< The component this decorator wraps.

	/**
	 * @brief Forwards the prepare step to the wrapped component.
	 * @param workUnit The unit being worked on.
	 */
	void prepare(WorkUnit* workUnit);

	/**
	 * @brief Forwards the perform step to the wrapped component.
	 * @param workUnit The unit being worked on.
	 */
	void perform(WorkUnit* workUnit);

	/**
	 * @brief Forwards the finish step to the wrapped component.
	 * @param workUnit The unit being worked on.
	 */
	void finish(WorkUnit* workUnit);

public:
	/** @brief Creates a decorator that wraps nothing yet. */
	EmployeeDetail();

	/**
	 * @brief Copy constructor.
	 * @param other The decorator to copy.
	 */
	EmployeeDetail(const EmployeeDetail& other);

	/** @brief Virtual destructor. Deletes the wrapped component. */
	virtual ~EmployeeDetail();

	/** @brief Gets the innermost employee by asking the wrapped component. */
	EmployeeComponent* getCore();

	/** @brief Gets the employee's name from the wrapped component. */
	string getName();

	/**
	 * @brief Checks the profession through the decorator chain.
	 * @param professionName Name of the profession.
	 * @return true if any decorator in the chain has this profession.
	 */
	bool compareProfession(string professionName);

	/**
	 * @brief Checks the competency through the decorator chain.
	 * @param requiredCompetency The level required.
	 * @return true if a decorator in the chain has this level.
	 */
	bool compareCompetency(int requiredCompetency);

	/**
	 * @brief Does one step of work, passing it through the decorator chain.
	 * @param workUnit The unit being worked on.
	 */
	void doWork(WorkUnit* workUnit);

	/**
	 * @brief Sets the mediator on this decorator and the wrapped component.
	 * @param mediator The mediator.
	 */
	void setMediator(Mediator* mediator);

	/**
	 * @brief Wraps a component.
	 * @param component The component to wrap. Ownership passes to this decorator.
	 */
	void wrap(EmployeeComponent* component);

	/** @brief Makes a deep copy of this decorator chain. Implemented by concrete decorators. */
	virtual EmployeeComponent* clone() = 0;
};

#endif
