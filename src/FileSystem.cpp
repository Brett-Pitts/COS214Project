#include "FileSystem.h"
#include <iostream>
void FileSystem::saveMemento(std::string filename, Memento* memento) {
	if (storage.find(filename) != storage.end()) {

	}
	storage[filename] = memento;
}

Memento* FileSystem::loadMemento(std::string filename) {
	auto iter = storage.find(filename);
	if (iter == storage.end()) {
		return nullptr;
	}
	return storage[filename];
}
