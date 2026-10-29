#ifndef MEMENTO_H
#define MEMENTO_H

#include "MementoType.h"
#include "ApplicationInterface.h"
#include "FileSystem.h"

class Memento {
	friend class ApplicationInterface;
	friend class FileSystem;
private:
	void* root;
	MementoType type;


	void setRoot(void* root, MementoType type);
public:
	MementoType getType() const;
	void* getRoot() const;
	~Memento();
	Memento(void* root, MementoType type);
};

#endif
