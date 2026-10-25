#ifndef TEAM_H
#define TEAM_H

#include <string>
#include <vector>
#include "CompanyNode.h"
#include "Mediator.h"

class EmployeeComponent;
class WorkUnit;

class Team : public CompanyNode, public Mediator{
private:
    std::vector<EmployeeComponent*> members;

public:
    void addMember(EmployeeComponent* employeeComponent);
    void assignWork(WorkUnit* workUnit);
    void notifyColleagues(EmployeeComponent* employeeComponent, std::string message) override;
};

#endif
