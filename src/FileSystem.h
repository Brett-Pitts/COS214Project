#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include <string>
#include <map>
#include <fstream>

class Memento;
class CompanyNode;
class WorkNode;

/**
 * @class FileSystem
 * @brief Stores mementos by name and saves them to, or loads them from, files.
 *
 * @par Design pattern
 * Memento (Caretaker). It keeps mementos without looking inside them,
 * except when writing them to a file (it is a friend of the employee classes
 * for that reason).
 *
 * @see Memento, ApplicationInterface
 */
class FileSystem {
private:
	std::map<std::string, Memento*> storage; ///< Saved mementos, keyed by name. Owned.

	/**
	 * @brief Writes a company tree to a file.
	 * @param file Open output stream.
	 * @param root Root of the company tree.
	 */
	void saveCompany(std::ofstream& file, CompanyNode* root);

	/**
	 * @brief Writes a work tree to a file.
	 * @param file Open output stream.
	 * @param root Root of the work tree.
	 */
	void saveWorkflow(std::ofstream& file, WorkNode* root);

	/**
	 * @brief Reads a company tree from a file.
	 * @param file Open input stream.
	 * @param[out] root Receives the new root of the company tree.
	 */
	void loadCompany(std::ifstream& file, CompanyNode*& root);

	/**
	 * @brief Reads a work tree from a file.
	 * @param file Open input stream.
	 * @param[out] root Receives the new root of the work tree.
	 */
	void loadWorkflow(std::ifstream& file, WorkNode*& root);
public:
	/** @brief Destroys the file system and every memento it holds. */
	~FileSystem();

	/**
	 * @brief Stores a memento under a name.
	 * @param name Key to store it under.
	 * @param memento The memento to store. Ownership passes to this object.
	 */
	void saveMemento(std::string name, Memento* memento);

	/**
	 * @brief Retrieves a stored memento.
	 * @param name The name it was stored under.
	 * @return The memento, or nullptr if there is none with that name.
	 */
	Memento* loadMemento(std::string name);

	/**
	 * @brief Writes a stored memento to a file.
	 * @param name Name of the stored memento.
	 * @param filename Path of the file to write.
	 */
	void saveToFile(std::string name, std::string filename);

	/**
	 * @brief Reads a file and stores the result as a memento.
	 * @param name Name to store the loaded memento under.
	 * @param filename Path of the file to read.
	 */
	void loadFromFile(std::string name, std::string filename);
};

#endif
