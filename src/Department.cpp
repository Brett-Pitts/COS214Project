#include "Department.h"

void Department::add(CompanyNode* companyNode) {
	if (companyNode != nullptr)
		children.push_back(companyNode);
}

void Department::remove(CompanyNode* companyNode) {
	if (!children.empty()) {
		for (auto it = children.begin(); it != children.end(); ++it) {

			if (*it == companyNode) {
				children.erase(it);
				return;
			}
		}
	}
}

CompanyNode* Department::getChild(int n) {
	if (n < 0 || n >= children.size()) {
		return nullptr;
	}
	return children.at(n);
}

CompanyNode* Department::clone() {
	Department* ret = new Department();

	for (auto& child : children) {
		ret->add(child->clone());
	}
	return ret;
}

Department::~Department() {
	if (!children.empty()) {
		for (auto it = children.begin(); it != children.end(); ++it) {

			delete* it;
		}
	}
}
