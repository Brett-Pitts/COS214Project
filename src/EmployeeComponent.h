#ifndef EMPLOYEECOMPONENT_H
#define EMPLOYEECOMPONENT_H

class EmployeeComponent {

private:
	Mediator mediator;
	EmployeeComponent successor;

public:
	bool compareProfession(int arg0);

	void doWork(WorkUnit workUnit);

protected:
	void finish(WorkUnit workUnit);

public:
	void handleWork(WorkUnit workUnit);

protected:
	void perform(WorkUnit workUnit);

	void prepare(WorkUnit workUnit);

public:
	void receive(string arg0);

	void setSuccessor(EmployeeComponent employeeComponent);

	virtual void wrap(EmployeeComponent component) = 0;

	EmployeeComponent* clone();
};

#endif
