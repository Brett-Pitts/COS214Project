#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include "Memento.h"
#include <map>
class FileSystem {
private:
	std::map<std::string, Memento*> storage;
	~FileSystem();
public:
	void saveMemento(std::string filename, Memento* memento);

	Memento* loadMemento(std::string filename);
};

#endif
