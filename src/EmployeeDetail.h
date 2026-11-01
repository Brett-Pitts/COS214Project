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
	 * @brief Does nothing; concrete decorators override it.
	 * @param workUnit The unit being worked on.
	 */
	void prepare(WorkUnit* workUnit);

	/**
	 * @brief Does nothing; concrete decorators override it.
	 * @param workUnit The unit being worked on.
	 */
	void perform(WorkUnit* workUnit);

	/**
	 * @brief Does nothing; concrete decorators override it.
	 * @param workUnit The unit being worked on.
	 */
	void finish(WorkUnit* workUnit);

public:
	/** @brief Creates a decorator that wraps nothing yet. */
	EmployeeDetail();

	/**
	 * @brief Copies the decorator and deep-copies what it wraps.
	 * @param other The decorator to copy.
	 */
	EmployeeDetail(const EmployeeDetail& other);

	/** @brief Deletes the wrapped component. */
	virtual ~EmployeeDetail();

	/**
	 * @brief Gets the innermost employee from the wrapped chain.
	 * @return The innermost component.
	 */
	EmployeeComponent* getCore();

	/**
	 * @brief Gets the name from the wrapped component (empty if none).
	 * @return The name.
	 */
	string getName();

	/**
	 * @brief Asks the wrapped component about the profession.
	 * @param professionName Profession to check.
	 * @return True if the profession matches.
	 */
	bool compareProfession(string professionName);

	/**
	 * @brief Asks the wrapped component about the competency.
	 * @param requiredCompetency The level required.
	 * @return True if the level is met.
	 */
	bool compareCompetency(CompetencyLevel requiredCompetency);


	/**
	 * @brief Gets the profession from the wrapped chain.
	 * @return The profession name.
	 */
	virtual std::string getProfession();

	/**
	 * @brief Gets the competency from the wrapped chain.
	 * @return The competency name.
	 */
	virtual std::string getCompetency();

	/**
	 * @brief Runs this layer's steps around the wrapped component's doWork().
	 * @param workUnit The unit being worked on.
	 */
	void doWork(WorkUnit* workUnit);

	/**
	 * @brief Sets the mediator on this decorator and what it wraps.
	 * @param mediator The mediator.
	 */
	void setMediator(Mediator* mediator);

	/**
	 * @brief Adds a component to the end of the chain.
	 * @param component The component to add to the chain.
	 */
	void wrap(EmployeeComponent* component);

	/**
	 * @brief Makes a deep copy of the chain; concrete decorators implement it.
	 * @return A new copy; the caller owns it.
	 */
	virtual EmployeeComponent* clone() = 0;
};

#endif
