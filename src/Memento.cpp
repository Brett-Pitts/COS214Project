#include "Memento.h"
#include <iostream>
Memento::Memento(void* root, MementoType type) {
	this->root = root;
	this->type = type;
}


MementoType  Memento::getType() const {
	return type;
}
void* Memento::getRoot() const {
	return root;
}
void  Memento::setRoot(void* root, MementoType type) {
	this->root = root;
	this->type = type;
}

Memento::~Memento() {
	if (type == MementoType::company) {
		delete (CompanyNode*)root;
	}
	else if (type == MementoType::workflow) {

		delete (WorkNode*)root;
	}
}
