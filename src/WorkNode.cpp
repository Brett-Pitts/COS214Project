#include "WorkNode.h"
#include "WorkState.h"
#include "WaitingForPrerequisites.h"
#include "Ready.h"
#include "Completed.h"
#include "Ongoing.h"
#include "Cancelled.h"
#include "Escalated.h"
#include "Failed.h"
#include "WorkIterator.h"
#include "WorkMediator.h"
#include <algorithm>

WorkNode::WorkNode(int maxRetries, std::string name) {
	this->assignee = NULL;
	this->maxRetries = maxRetries;
	this->mediator = NULL;
	this->parent = NULL;
	this->retryCount = 0;
	this->state = NULL;
	this->setState(new WaitingForPrerequisites());
	this->name = name;
}

void WorkNode::setName(std::string name) {
	if (!hasName(name)) {
		this->name = name;
	}
	else {
		setName(name + "1");
	}
}

WorkNode::~WorkNode() {
	delete this->state;
}

std::string WorkNode::getStateName() {
	return state->getName();
}

const std::vector<std::string>& WorkNode::getPrerequisites() const {
	return prerequisites;
}

void WorkNode::addPrerequisite(std::string workName) {
	this->prerequisites.push_back(workName);
}

void WorkNode::advanceState(){
	if(this->state == NULL){
		return;
	}
	bool wasCompleted = isCompleted();
	this->state->handle(this);
	if(!wasCompleted && isCompleted() && this->mediator != NULL){
		this->mediator->notifyColleagues(this, this->name);
	}
}

bool WorkNode::allChildrenCompleted() {
	return true;
}

WorkIterator* WorkNode::createIterator() {
	return new WorkIterator(this);
}

WorkNode* WorkNode::getChild(int index) {
	return nullptr;
}

WorkNode* WorkNode::getParent() {
	return this->parent;
}

bool WorkNode::hasRetriesLeft() {
	return this->retryCount < this->maxRetries;
}

bool WorkNode::isCompleted() {
	return dynamic_cast<Completed*>(this->state) != NULL;
}

bool WorkNode::isOngoing() {
	return dynamic_cast<Ongoing*>(this->state) != NULL;
}

bool WorkNode::isReady() {
	return dynamic_cast<Ready*>(this->state) != NULL;
}

void WorkNode::modify() {
	// TODO - implement WorkNode::modify
	throw "Not yet implemented";
}

bool WorkNode::prerequisitesCompleted() {
	return this->prerequisites.size() == 0;
}

void WorkNode::reassign(EmployeeComponent* employeeComponent) {
	this->assignee = employeeComponent;
}

void WorkNode::remove(WorkNode* workNode) {
	return;
}

void WorkNode::reset() {
	this->retryCount++;
	this->setState(new Ready());
}

void WorkNode::setMediator(WorkMediator* mediator) {
	this->mediator = mediator;
}

void WorkNode::setParent(WorkNode* parent) {
	this->parent = parent;
}


void WorkNode::setState(WorkState* workState) {
	if (workState == nullptr) {
		return;
	}
	if (this->state != nullptr) {
		delete this->state;
	}
	this->state = workState;
}

void WorkNode::setState(std::string stateName) {
	if (stateName == "WaitingForPrerequisites") {
		setState(new WaitingForPrerequisites());
	}
	if (stateName == "Ready") {
		setState(new Ready());
	}
	if (stateName == "Ongoing") {
		setState(new Ongoing());
	}
	if (stateName == "Completed") {
		setState(new Completed());
	}
	if (stateName == "Cancelled") {
		setState(new Cancelled());
	}
	if (stateName == "Escalated") {
		setState(new Escalated());
	}
	if (stateName == "Failed") {
		setState(new Failed());
	}

}

void WorkNode::update(std::string completedName){
	int before = prerequisites.size();
	prerequisites.erase(std::remove(prerequisites.begin(), prerequisites.end(), completedName), prerequisites.end());
	if(prerequisites.size() == before){
		return;
	}
	if(prerequisites.empty() && getStateName() == "WaitingForPrerequisites"){
		advanceState();
	}
}

int WorkNode::getMaxRetries() {
	return maxRetries;
}

std::string WorkNode::getName() {
	return name;
}
