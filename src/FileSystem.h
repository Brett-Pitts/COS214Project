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

class FileSystem {
private:
	std::map<std::string, Memento*> storage;

	/**
	 * @brief Writes the company tree to the stream.
	 * @param file Stream to write to.
	 * @param root Root of the tree.
	 */
	void saveCompany(std::ostream& file, CompanyNode* root);
	// for gui:
	void saveMember(std::ostream& file, EmployeeComponent* member);

	/**
	 * @brief Writes the work tree to the stream.
	 * @param file Stream to write to.
	 * @param root Root of the tree.
	 */
	void saveWorkflow(std::ostream& file, WorkNode* root);
	// for gui:
	void savePrerequisites(std::ostream& file, WorkNode* node);

	/**
	 * @brief Reads a company tree from the stream.
	 * @param file Stream to read from.
	 * @param tag The tag line already read.
	 * @return The company tree, or nullptr on failure.
	 */
	CompanyNode* loadCompany(std::istream& file, const std::string& tag);
	// for gui:
	EmployeeComponent* loadMember(std::istream& file);

	/**
	 * @brief Reads a work tree from the stream.
	 * @param file Stream to read from.
	 * @param tag The tag line already read.
	 * @return The work tree, or nullptr on failure.
	 */
	WorkNode* loadWorkflow(std::istream& file, const std::string& tag);
	// for gui:
	bool loadPrerequisites(std::istream& file, WorkNode* node);

public:
	/** @brief Deletes every stored memento. */
	~FileSystem();

	/**
	 * @brief Stores a memento under a name, deleting any older one with that name.
	 * @param name Key to store it under.
	 * @param memento The memento (now owned).
	 */
	void saveMemento(std::string name, Memento* memento);

	/**
	 * @brief Gets the memento stored under a name, or nullptr.
	 * @param name Key to look up.
	 * @return The memento, or nullptr if there is none.
	 */
	Memento* loadMemento(std::string name);

	/**
	 * @brief Writes a stored memento to a file (saves/ folder and .txt by default).
	 * @param name Key of the memento to write.
	 * @param filename File to write to.
	 * @return True on success.
	 */
	bool saveToFile(std::string name, std::string filename);

	/**
	 * @brief Reads a file into a memento stored under the name; false on failure.
	 * @param name Key to store the memento under.
	 * @param filename File to read.
	 * @return True on success.
	 */
	bool loadFromFile(std::string name, std::string filename);
};

#endif
