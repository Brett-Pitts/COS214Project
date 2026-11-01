#ifndef EMPLOYEECOMPONENT_H
#define EMPLOYEECOMPONENT_H

#include <string>

#include "CompetencyLevel.h"
using std::string;


class Mediator;
class WorkUnit;
class FileSystem;

/** @brief Base of every employee and decorator. Members of a team are linked as a chain. */
class EmployeeComponent {
	friend class FileSystem;
private:
	/** The team this employee talks through. Not owned. */
	Mediator* mediator;
	/** Next employee in the chain. Not owned. */
	EmployeeComponent* successor;

	/** @brief Checks if the competency is high enough for the unit's current state. */
	bool canAdvance(WorkUnit* workUnit);

protected:
	/** @brief First step of doWork(). Prints a line. */
	virtual void prepare(WorkUnit* workUnit);
	/** @brief Middle step of doWork(). Prints a line. */
	virtual void perform(WorkUnit* workUnit);
	/** @brief Last step of doWork(). Prints a line. */
	virtual void finish(WorkUnit* workUnit);

public:
	/** @brief Creates an employee with no mediator or successor. */
	EmployeeComponent();
	/** @brief Copy constructor. */
	EmployeeComponent(const EmployeeComponent& other);
	/** @brief Virtual destructor. */
	virtual ~EmployeeComponent();
	virtual EmployeeComponent* getCore();
	/** @brief Gets the kind of component. */
	virtual std::string getKind() = 0;
	/** @brief Gets the employee's name. */
	virtual string getName() = 0;
	/** @brief Checks for a profession. False by default. */
	virtual bool compareProfession(string professionName);
	/** @brief Checks for a competency level. False by default. */
	virtual bool compareCompetency(CompetencyLevel requiredCompetency);
	/** @brief Gets the profession name. Empty by default. */
	virtual std::string getProfession();
	/** @brief Gets the competency name. Empty by default. */
	virtual std::string getCompetency();
	/** @brief Prepares, performs and finishes the unit, then tells the mediator. */
	virtual void doWork(WorkUnit* workUnit);
	/** @brief Advances the unit if able, otherwise passes it on. True if someone advanced it. */
	bool handleWork(WorkUnit* workUnit);
	/** @brief Prints a message from a teammate. */
	virtual void receive(string message);
	/** @brief Sets the next employee in the chain. */
	void setSuccessor(EmployeeComponent* employeeComponent);
	/** @brief Sets the mediator. */
	virtual void setMediator(Mediator* mediator);
	/** @brief Wraps another component. */
	virtual void wrap(EmployeeComponent* component) = 0;
	/** @brief Makes a copy. */
	virtual EmployeeComponent* clone() = 0;
};

#endif
