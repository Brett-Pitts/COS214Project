#ifndef FILESYSTEM_H
#define FILESYSTEM_H

class FileSystem {


public:
	void saveMemento(string filename, Memento* memento);

	Memento* loadMemento(string filename);
};

#endif
