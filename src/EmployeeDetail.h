#ifndef EMPLOYEEDETAIL_H
#define EMPLOYEEDETAIL_H

#include "EmployeeComponent.h"

class EmployeeDetail : public EmployeeComponent {

private:
	EmployeeComponent* wrapped;

protected:
	void prepare(WorkUnit* workUnit);
	void perform(WorkUnit* workUnit);
	void finish(WorkUnit* workUnit);

public:
	EmployeeDetail();
	EmployeeDetail(const EmployeeDetail& other);
	virtual ~EmployeeDetail();

	string getName();
	bool compareProfession(int professionId);
	void doWork(WorkUnit* workUnit);
	void setMediator(Mediator* mediator);
	void wrap(EmployeeComponent* component);
};

#endif
