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
	void saveMemento(std::string name, Memento* memento);

	Memento* loadMemento(std::string name);

	void saveToFile(std::string name, std::string filename);
	void loadFromFile(std::string name, std::string filename);

};

#endif
