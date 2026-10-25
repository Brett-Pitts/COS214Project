#ifndef FILESYSTEM_H
#define FILESYSTEM_H

class FileSystem {


public:
	void saveMemento(std::string filename, Memento* memento);

	Memento* loadMemento(std::string filename);
};

#endif
