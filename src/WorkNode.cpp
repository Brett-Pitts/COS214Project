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
	this->assignee = nullptr;
	this->maxRetries = maxRetries;
	this->mediator = nullptr;
	this->parent = nullptr;
	this->retryCount = 0;
	this->state = nullptr;
	this->setState(new Ready());
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
WorkNode* WorkNode::getRoot() {
	if (this->parent != nullptr) {
		return this->parent->getRoot();
	}
	return this;
}

WorkNode::~WorkNode() {
	delete this->state;
}

std::string WorkNode::getStateName() {
	if (state == nullptr) {
		return "";
	}
	return state->getName();
}

const std::vector<std::string>& WorkNode::getPrerequisites() const {
	return prerequisites;
}

void WorkNode::addPrerequisite(std::string workName) {
	if (this->isCompleted() || this->getStateName() == "Failed") {
		return;
	}
	if (getRoot()->getNode(workName) != nullptr) {

		if (getRoot()->getNode(workName)->isCompleted()) {
			return;
		}
	}
	this->prerequisites.push_back(workName);

	if (this->getStateName() != "WaitingForPrerequisites") {
		this->setState(new WaitingForPrerequisites());
	}
}

void WorkNode::advanceState() {
	if (this->state == NULL) {
		return;
	}

	this->state->handle(this);
	if (isCompleted() && this->mediator != NULL) {
		this->mediator->notifyColleagues(this, this->name);
	}
}

bool WorkNode::allChildrenCompleted() {
	return true;
}

WorkIterator* WorkNode::createIterator() {
	return new WorkIterator(this);
}

WorkNode* WorkNode::getChild(int) {
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

bool WorkNode::isWaitingForPrerequisites() {

	if (this->parent) {
		if (this->parent->isWaitingForPrerequisites()) {
			return true;
		}
	}
	return !(this->prerequisites.empty());
}
bool WorkNode::isEscalated() {
	return dynamic_cast<Escalated*>(this->state) != NULL;
}

bool WorkNode::isReady() {
	return dynamic_cast<Ready*>(this->state) != NULL;
}

void WorkNode::modify() {
	// TODO - implement WorkNode::modify
	throw "Not yet implemented";
}





void WorkNode::reassign(EmployeeComponent* employeeComponent) {
	this->assignee = employeeComponent;
}

void WorkNode::remove(WorkNode*) {
	return;
}

void WorkNode::reset() {
	if (this->isOngoing() || this->getStateName() == "Escalated") {

		this->retryCount++;
		this->setState(new Ready());
	}
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

void WorkNode::update(std::string completedName) {
	prerequisites.erase(
		std::remove(prerequisites.begin(), prerequisites.end(), completedName),
		prerequisites.end());
	refresh();   // always re-check, even if nothing was erased
}

int WorkNode::getMaxRetries() {
	return maxRetries;
}

std::string WorkNode::getName() {
	return name;
}

// for gui:
void WorkNode::copyStateFrom(WorkNode* other) {
	if (other == nullptr) {
		return;
	}
	this->prerequisites = other->prerequisites;
	this->retryCount = other->retryCount;
	this->setState(other->getStateName());
}

// for gui:
void WorkNode::setMaxRetries(int retries) {
	if (retries >= 0) {
		this->maxRetries = retries;
	}
}

// for gui:
void WorkNode::removePrerequisite(std::string workName) {
	prerequisites.erase(std::remove(prerequisites.begin(), prerequisites.end(), workName), prerequisites.end());
}

void WorkNode::refresh() {
	if (getStateName() == "WaitingForPrerequisites" && !isWaitingForPrerequisites()) {
		setState(new Ready());
	}
}