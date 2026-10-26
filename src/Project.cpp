#include "Project.h"
#include <algorithm>

Project::Project(int maxRetries) : WorkNode(maxRetries) {}

Project::~Project() {

    for (WorkNode* child : children)
        delete child;

}

void Project::add(WorkNode* workNode) {

    if (workNode == NULL) return;
    workNode->setParent(this);
    children.push_back(workNode);

}

void Project::remove(WorkNode* workNode) {
    std::vector<WorkNode*>::iterator it =std::find(children.begin(), children.end(), workNode);

    if (it != children.end()) {
        (*it)->setParent(NULL);
        children.erase(it);
    }

}

WorkNode* Project::getChild(int index) {

    if (index < 0 || index >= (int)children.size()) return NULL;
    return children[index];

}

bool Project::allChildrenCompleted() {

    for (int i = 0; i < children.size(); i++)
        if (!children[i]->isCompleted()) return false;
    return true;

}

WorkNode* Project::clone() {

    Project* copy = new Project(this->getMaxRetries());
    for (int i = 0; i < children.size(); i++)
        copy->add(children[i]->clone());
    return copy;

}