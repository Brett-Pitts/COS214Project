#ifndef MEMENTO_H
#define MEMENTO_H

#include "MementoType.h"
#include "ApplicationInterface.h"
#include "FileSystem.h"

class Memento {
	friend ApplicationInterface;
	friend FileSystem;
private:
	void* root;
	MementoType type;

	MementoType getType() const;
	void* getRoot() const;

	void setRoot(void* root, MementoType type);
public:
	~Memento();
	Memento(void* root, MementoType type);
};

#endif
