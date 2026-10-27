#ifndef EMPLOYEECOMPONENT_H
#define EMPLOYEECOMPONENT_H

#include <string>

using namespace std;

class Mediator;
class WorkUnit;
class FileSystem;

class EmployeeComponent {
	friend class FileSystem;
private:
	Mediator* mediator;
	EmployeeComponent* successor;

protected:
	virtual void prepare(WorkUnit* workUnit);
	virtual void perform(WorkUnit* workUnit);
	virtual void finish(WorkUnit* workUnit);

public:
	EmployeeComponent();
	EmployeeComponent(const EmployeeComponent& other);
	virtual ~EmployeeComponent();

	virtual std::string getKind() = 0;
	virtual string getName() = 0;
	virtual bool compareProfession(int professionId);
	virtual void doWork(WorkUnit* workUnit);
	void handleWork(WorkUnit* workUnit);
	virtual void receive(string message);
	void setSuccessor(EmployeeComponent* employeeComponent);
	virtual void setMediator(Mediator* mediator);
	virtual void wrap(EmployeeComponent* component) = 0;
	virtual EmployeeComponent* clone() = 0;
};

#endif