#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <map>
#include <iostream>
#include <fstream>

class Memento;
class CompanyNode;
class WorkNode;
class EmployeeComponent;

/** @brief Keeps named mementos in memory and saves or loads them as text files. */
class FileSystem {
private:
	/** Stored mementos by name. Owned. */
	std::map<std::string, Memento*> storage;

	/** @brief Writes a company tree to a stream. */
	void saveCompany(std::ostream& file, CompanyNode* root);
	// for gui:
	void saveMember(std::ostream& file, EmployeeComponent* member);
	/** @brief Writes a work tree to a stream. */
	void saveWorkflow(std::ostream& file, WorkNode* root);
	// for gui:
	void savePrerequisites(std::ostream& file, WorkNode* node);

	/** @brief Reads a company tree from a stream. nullptr on failure. */
	CompanyNode* loadCompany(std::istream& file, const std::string& tag);
	// for gui:
	EmployeeComponent* loadMember(std::istream& file);
	/** @brief Reads a work tree from a stream. nullptr on failure. */
	WorkNode* loadWorkflow(std::istream& file, const std::string& tag);
	// for gui:
	bool loadPrerequisites(std::istream& file, WorkNode* node);

public:
	/** @brief Deletes all stored mementos. */
	~FileSystem();
	/** @brief Stores a memento under a name. An older one with that name is deleted. */
	void saveMemento(std::string name, Memento* memento);

	/** @brief Gets a stored memento, or nullptr. Still owned by the file system. */
	Memento* loadMemento(std::string name);

	/** @brief Writes a stored memento to a file. False on failure. */
	bool saveToFile(std::string name, std::string filename);
	/** @brief Reads a file into a memento stored under the name. False on failure. */
	bool loadFromFile(std::string name, std::string filename);
};

#endif
