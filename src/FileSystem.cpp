#include "FileSystem.h"
#include <iostream>
#include <fstream>
#include "MementoType.h"
FileSystem::~FileSystem() {
	for (const auto& pair : storage) {
		delete pair.second;
	}


}
void FileSystem::saveMemento(std::string name, Memento* memento) {
	if (storage.find(name) != storage.end()) {

	}
	storage[name] = memento;
}

Memento* FileSystem::loadMemento(std::string name) {
	auto iter = storage.find(name);
	if (iter == storage.end()) {
		return nullptr;
	}
	return storage[name];
}

void FileSystem::saveToFile(std::string name, std::string filename) {

	Memento* mem = loadMemento(name);
	if (mem == nullptr) {
		return;
	}

	std::ofstream file("saves/" + filename + ".txt");
	if (!file.is_open()) {
		std::cerr << "Failed to open file: " << filename << '\n';
		return;
	}

	file.clear();

	switch (mem->getType()) {
	case (MementoType::company):
	{
		file << "company\n";



		break;
	}
	case (MementoType::workflow):
	{


		break;
	}

	default:
		break;
	}



}

void FileSystem::loadFromFile(std::string name, std::string filename) {}
