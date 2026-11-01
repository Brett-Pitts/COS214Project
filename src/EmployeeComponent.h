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

	bool canAdvance(WorkUnit* workUnit);

protected:
	virtual void prepare(WorkUnit* workUnit);
	virtual void perform(WorkUnit* workUnit);
	virtual void finish(WorkUnit* workUnit);

public:
	EmployeeComponent();
	EmployeeComponent(const EmployeeComponent& other);
	virtual ~EmployeeComponent();
	virtual EmployeeComponent* getCore();
	virtual std::string getKind() = 0;
	virtual string getName() = 0;
	virtual bool compareProfession(string professionName);
	virtual bool compareCompetency(CompetencyLevel requiredCompetency);
	virtual std::string getProfession();
	virtual std::string getCompetency();
	virtual void doWork(WorkUnit* workUnit);
	bool handleWork(WorkUnit* workUnit);
	virtual void receive(string message);
	void setSuccessor(EmployeeComponent* employeeComponent);
	virtual void setMediator(Mediator* mediator);
	virtual void wrap(EmployeeComponent* component) = 0;
	virtual EmployeeComponent* clone() = 0;
};

#endif
