#include "EmployeeComponent.h"

EmployeeComponent::~EmployeeComponent(){
}

bool EmployeeComponent::compareProfession(int id){
    throw "Not yet implemented";
}

void EmployeeComponent::doWork(WorkUnit* workUnit){
    throw "Not yet implemented";
}

void EmployeeComponent::finish(WorkUnit* workUnit){
    throw "Not yet implemented";
}

void EmployeeComponent::handleWork(WorkUnit* workUnit){
    throw "Not yet implemented";
}

void EmployeeComponent::perform(WorkUnit* workUnit){
    throw "Not yet implemented";
}

void EmployeeComponent::prepare(WorkUnit* workUnit){
    throw "Not yet implemented";
}

void EmployeeComponent::receive(std::string message){
    throw "Not yet implemented";
}

void EmployeeComponent::setMediator(Mediator* mediator){
    throw "Not yet implemented";
}

void EmployeeComponent::setSuccessor(EmployeeComponent* employeeComponent){
    this->successor = employeeComponent;
}