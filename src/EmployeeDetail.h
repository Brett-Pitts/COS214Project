#ifndef EMPLOYEEDETAIL_H
#define EMPLOYEEDETAIL_H
#include "CompetencyLevel.h"
#include "EmployeeComponent.h"
class FileSystem;

/**
 * @class EmployeeDetail
 * @brief Abstract decorator that adds a detail (profession or competency) to an employee.
 *
 * Holds the component it wraps, takes ownership of it and deletes it in the
 * destructor. The name, core, profession and competency queries, setMediator()
 * and doWork() are passed on to the wrapped component, so concrete decorators
 * only add their own part.
 *
 * @par Design patterns
 * - Decorator (Decorator).
 * - Template Method: doWork() runs this decorator's prepare() and perform(), then
 *   the wrapped component's doWork(), then this decorator's finish(). The versions
 *   of prepare(), perform() and finish() in this class do nothing; concrete
 *   decorators override them.
 *
 * @see EmployeeProfession, EmployeeCompetency
 */
class EmployeeDetail : public EmployeeComponent {
	friend class FileSystem;
private:

protected:
	EmployeeComponent* wrapped; ///< The component this decorator wraps, or nullptr until wrap() is called. Owned.

	/**
	 * @brief Does nothing. Concrete decorators override this to add their part.
	 * @param workUnit The unit being worked on.
	 */
	void prepare(WorkUnit* workUnit);

	/**
	 * @brief Does nothing. Concrete decorators override this to add their part.
	 * @param workUnit The unit being worked on.
	 */
	void perform(WorkUnit* workUnit);

	/**
	 * @brief Does nothing. Concrete decorators override this to add their part.
	 * @param workUnit The unit being worked on.
	 */
	void finish(WorkUnit* workUnit);

public:
	/** @brief Creates a decorator that wraps nothing yet. */
	EmployeeDetail();

	/**
	 * @brief Copy constructor. Copies the mediator and successor pointers and makes a deep copy (clone()) of the wrapped component.
	 * @param other The decorator to copy.
	 */
	EmployeeDetail(const EmployeeDetail& other);

	/** @brief Virtual destructor. Deletes the wrapped component. */
	virtual ~EmployeeDetail();

	/**
	 * @brief Gets the innermost employee by asking the wrapped component.
	 * @return The core of the wrapped chain, or this decorator if nothing is wrapped.
	 */
	EmployeeComponent* getCore();

	/**
	 * @brief Gets the employee's name from the wrapped component, or an empty string if nothing is wrapped.
	 */
	string getName();

	/**
	 * @brief Checks the profession through the decorator chain by asking the wrapped component.
	 *
	 * The decorator must wrap something; this is not checked.
	 *
	 * @param professionName Name of the profession.
	 * @return true if a decorator further in the chain has this profession.
	 */
	bool compareProfession(string professionName);

	/**
	 * @brief Checks the competency through the decorator chain by asking the wrapped component.
	 * @param requiredCompetency The level required.
	 * @return true if a decorator further in the chain has at least this level. If nothing is wrapped, false.
	 */
	bool compareCompetency(CompetencyLevel requiredCompetency);


	/** @brief Gets the profession from the wrapped component. */
	virtual std::string getProfession();
	/** @brief Gets the competency from the wrapped component. */
	virtual std::string getCompetency();

	/**
	 * @brief Does the work in layers.
	 *
	 * Runs this decorator's prepare() and perform(), then the wrapped component's
	 * doWork(), then this decorator's finish(). The steps of the outermost decorator
	 * therefore come first and last, and the core employee's steps (and the mediator
	 * notification) are in the middle. A null unit is ignored.
	 *
	 * @param workUnit The unit being worked on.
	 */
	void doWork(WorkUnit* workUnit);

	/**
	 * @brief Sets the mediator on this decorator and the wrapped component.
	 * @param mediator The mediator.
	 */
	void setMediator(Mediator* mediator);

	/**
	 * @brief Adds a component to the end of the decorator chain.
	 *
	 * If this decorator wraps nothing yet, the component becomes the wrapped
	 * component. Otherwise the call is passed down to the wrapped component. A null
	 * pointer and the decorator itself are ignored. The mediator is not set on the new
	 * component.
	 *
	 * @note If the chain already ends in an Employee or Manager, Employee::wrap()
	 * deletes the component. Wrap the core last.
	 *
	 * @param component The component to add. Ownership passes to the chain.
	 */
	void wrap(EmployeeComponent* component);

	/** @brief Makes a deep copy of this decorator chain. Implemented by concrete decorators. */
	virtual EmployeeComponent* clone() = 0;
};

#endif
