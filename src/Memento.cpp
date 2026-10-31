#include "Memento.h"
#include "CompanyNode.h"
#include "WorkNode.h"

Memento::Memento(void* root, MementoType type) {
	this->root = root;
	this->type = type;
}

MementoType Memento::getType() const {
	return type;
}

void* Memento::getRoot() const {
	return root;
}

void Memento::setRoot(void* root, MementoType type) {
	if (this->root != nullptr && this->root != root) {
		if (this->type == MementoType::company) {
			delete (CompanyNode*)this->root;
		}
		else {
			delete (WorkNode*)this->root;
		}
	}
	this->root = root;
	this->type = type;
}

Memento::~Memento() {
	if (root == nullptr) {
		return;
	}
	if (type == MementoType::company) {
		delete (CompanyNode*)root;
	}
	else {
		delete (WorkNode*)root;
	}
}
