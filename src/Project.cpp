#include "Project.h"
#include <algorithm>
#include "WorkNode.h"
#include "WaitingForPrerequisites.h"

Project::Project(int maxRetries, std::string name) : WorkNode(maxRetries, name) {}

Project::~Project() {

	for (WorkNode* child : children)
		delete child;

}
#include <iostream>
void Project::add(WorkNode* workNode) {
	if (workNode == nullptr) return;
	if (getRoot()->hasName(workNode->getName())) {
		workNode->setName(workNode->getName() + "1");
	}
	workNode->setParent(this);
	if (this->isWaitingForPrerequisites()) {
		workNode->setState(new WaitingForPrerequisites());
	}
	children.push_back(workNode);

}

void Project::remove(WorkNode* workNode) {
	std::vector<WorkNode*>::iterator it = std::find(children.begin(), children.end(), workNode);

	if (it != children.end()) {
		(*it)->setParent(NULL);
		children.erase(it);
	}

}

WorkNode* Project::getChild(int index) {

	if (index < 0 || index >= (int)children.size()) return nullptr;
	return children[index];

}

bool Project::allChildrenCompleted() {

	for (int i = 0; i < (int)children.size(); i++)
		if (!children[i]->isCompleted()) return false;
	return true;

}

WorkNode* Project::clone() {

	Project* copy = new Project(this->getMaxRetries(), this->getName());
	for (int i = 0; i < (int)children.size(); i++)
		copy->add(children[i]->clone());
	copy->copyStateFrom(this);
	return copy;

}

WorkNode* Project::getNode(std::string name) {
	WorkNode* node = nullptr;
	for (int i = 0;i < (int)this->children.size();i++) {
		node = children[i]->getNode(name);
		if (node != nullptr) {
			break;
		}
	}
	return node;
}

bool Project::hasName(std::string name) {
	if (this->name == name) {
		return true;
	}

	for (auto child : children) {
		if (child->hasName(name)) {
			return true;
		}
	}
	return false;
}


void Project::update(std::string completedName)
{
    WorkNode::update(completedName);           // erases prerequisite + calls refresh()

    if (getStateName() == "Ready")
    {
        for (auto child : children)
            if (child->getStateName() != "Completed")
                return;
        setState("Escalated");
        advanceState();
    }
}

void Project::refresh() {
    WorkNode::refresh();                      
    if (!isWaitingForPrerequisites()) 
	{
        for (auto child : children) {
            child->refresh();                  
        }
    }
}
