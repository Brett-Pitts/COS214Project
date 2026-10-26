#ifndef MEMENTO_H
#define MEMENTO_H

#include "MementoType.h"
#include "ApplicationInterface.h"

class Memento {
	friend ApplicationInterface;
private:
	void* root;
	MementoType type;

	MementoType getType() const;
	void* getRoot() const;

	void setRoot(void* root, MementoType type);
public:
	Memento(void* root, MementoType type);
};

#endif
