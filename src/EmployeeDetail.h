#ifndef EMPLOYEEDETAIL_H
#define EMPLOYEEDETAIL_H

#include "EmployeeComponent.h"
class FileSystem;
class EmployeeDetail : public EmployeeComponent {
	friend class FileSystem;
private:

protected:
	EmployeeComponent* wrapped;
	void prepare(WorkUnit* workUnit);
	void perform(WorkUnit* workUnit);
	void finish(WorkUnit* workUnit);

public:
	EmployeeDetail();
	EmployeeDetail(const EmployeeDetail& other);
	virtual ~EmployeeDetail();

	std::string getName();
	bool compareProfession(std::string professionName);
	void doWork(WorkUnit* workUnit);
	void setMediator(Mediator* mediator);
	void wrap(EmployeeComponent* component);
	virtual EmployeeComponent* clone() = 0;
};

#endif
