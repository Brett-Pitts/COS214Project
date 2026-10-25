#ifndef EMPLOYEECOMPONENT_H
#define EMPLOYEECOMPONENT_H

#include <string>

class Mediator;
class WorkUnit;

class EmployeeComponent{

private:
    Mediator* mediator = NULL;
    EmployeeComponent* successor = NULL;

public:
    virtual ~EmployeeComponent();

    virtual bool compareProfession(int id);

    virtual void doWork(WorkUnit* workUnit);

    void handleWork(WorkUnit* workUnit);

    void receive(std::string message);

    void setMediator(Mediator* mediator);

    void setSuccessor(EmployeeComponent* employeeComponent);

    virtual void wrap(EmployeeComponent* component) = 0;

    virtual EmployeeComponent* clone() = 0;

protected:
    virtual void finish(WorkUnit* workUnit);

    virtual void perform(WorkUnit* workUnit);

    virtual void prepare(WorkUnit* workUnit);
};

#endif