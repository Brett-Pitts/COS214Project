#include "WorkNode.h"
#include "WorkState.h"
#include "WaitingForPrerequisites.h"
#include "Ready.h"
#include "Completed.h"
#include "WorkIterator.h"

WorkNode::WorkNode(int maxRetries){
    this->assignee = NULL;
    this->maxRetries = maxRetries;
    this->mediator = NULL;
    this->parent = NULL;
    this->retryCount = 0;
    this->state = NULL;
    this->setState(new WaitingForPrerequisites());
}

WorkNode::~WorkNode(){
    delete this->state;
}

void WorkNode::addPrerequisite(WorkNode* workNode){
    this->prerequisites.push_back(workNode);
}

void WorkNode::advanceState(){
    if(this->state != NULL){
        this->state->handle(this);
    }
}

bool WorkNode::allChildrenCompleted(){
    return true;
}

WorkIterator WorkNode::createIterator(){
    // TODO - implement WorkNode::createIterator
    throw "Not yet implemented";
}

WorkNode* WorkNode::getChild(int index){
    return nullptr;
}

WorkNode* WorkNode::getParent(){
    return this->parent;
}

bool WorkNode::hasRetriesLeft(){
    return this->retryCount < this->maxRetries;
}

bool WorkNode::isCompleted(){
    return dynamic_cast<Completed*>(this->state) != NULL;
}

void WorkNode::modify(){
    // TODO - implement WorkNode::modify
    throw "Not yet implemented";
}

bool WorkNode::prerequisitesCompleted(){
    int count = this->prerequisites.size();
    for(int i = 0; i < count; i++){
        if(!this->prerequisites[i]->isCompleted()){
            return false;
        }
    }
    return true;
}

void WorkNode::reassign(EmployeeComponent* employeeComponent){
    this->assignee = employeeComponent;
}

void WorkNode::remove(WorkNode* workNode){
  return ;
}

void WorkNode::reset(){
    this->retryCount++;
    this->setState(new Ready());
}

void WorkNode::setMediator(WorkMediator* mediator){
    this->mediator = mediator;
}

void WorkNode::setParent(WorkNode* parent){
    this->parent = parent;
}


void WorkNode::setState(WorkState* workState){
    WorkState* oldState = this->state;
    this->state = workState;
    delete oldState;
}

void WorkNode::update(){
    // TODO - implement WorkNode::update
    throw "Not yet implemented";
}

int WorkNode::getMaxRetries() {
    return maxRetries;
}