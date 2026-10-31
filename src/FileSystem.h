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
 * Mementos are kept in memory under a name and are owned by the FileSystem.
 * A stored memento can be written to a text file, and a file can be read back
 * into a stored memento. Files are in the `saves` folder (which must exist, and
 * is relative to the working directory) and end in `.tvd`.
 *
 * @par File format
 * The file is plain text with one item per line. The first line is `Company` or
 * `Workflow`.
 * - Company: a department is `Department`, its children and `EndDepartment`. A
 *   team is `Team`, its profession, one `Member` ... `EndMember` block per employee
 *   and `EndTeam`. A member lists its parts from the outermost decorator to the
 *   core: `Profession`, `Competency`, `Employee` or `Manager`, each followed by
 *   its name and an End line such as `EndProfession`.
 * - Workflow: a project is `Project`, its name, retry limit and state, a
 *   `Prerequisites` ... `EndPrerequisites` block, then `Children` ... `EndChildren`
 *   and `EndProject`. A work unit is `Unit`, its name, required profession and
 *   state, a prerequisites block and `EndUnit`.
 *
 * The retry counts, assignees, mediators and the links between the employees of a
 * team are not saved. Team::addMember() sets the links again when a team is loaded.
 *
 * @par Design pattern
 * Memento (Caretaker). It keeps mementos without looking inside them, except
 * when writing them to a file or reading them from one (it is a friend of Team,
 * WorkNode and the employee classes for that reason).
 *
 * @see Memento, ApplicationInterface
 */
class FileSystem {
private:
	std::map<std::string, Memento*> storage; ///< Saved mementos, keyed by name. Owned.

	/**
	 * @brief Writes a company tree to a file.
	 *
	 * A Team is written with its profession and with the parts of each member, from
	 * the outermost decorator to the core. Any other node is written as a Department
	 * followed by its children. A null root writes nothing.
	 *
	 * @param file Open output stream.
	 * @param root Root of the company tree.
	 */
	void saveCompany(std::ofstream& file, CompanyNode* root);

	/**
	 * @brief Writes a work tree to a file.
	 *
	 * A WorkUnit is written with its name, required profession, state and outstanding
	 * prerequisites. Any other node is written as a Project with its name, retry
	 * limit, state, prerequisites and children. A null root writes nothing.
	 *
	 * @param file Open output stream.
	 * @param root Root of the work tree.
	 */
	void saveWorkflow(std::ofstream& file, WorkNode* root);

	/**
	 * @brief Reads one company node, and everything below it, from a file.
	 *
	 * For a department, children are read until one cannot be read, which is the case at
	 * the line `EndDepartment`. For a team, the profession and the members are read;
	 * each member is built by wrapping the parts in the order they are read, and is
	 * added with Team::addMember(). A member that is rejected is deleted and a message
	 * is printed.
	 *
	 * @param file Open input stream.
	 * @param[out] root Receives the new node. It is left unchanged at an end marker, at
	 *        the end of the file and when the line is not recognised.
	 */
	void loadCompany(std::ifstream& file, CompanyNode*& root);

	/**
	 * @brief Reads one work node, and everything below it, from a file.
	 *
	 * Reads a project (header, prerequisites and children) or a work unit. The state
	 * is applied with WorkNode::setState(), so an unknown state name leaves the node
	 * in WaitingForPrerequisites. A work unit without a state and prerequisites, as
	 * in older files, is accepted. Problems in the file are reported on the error
	 * stream. Mediators are not set.
	 *
	 * @param file Open input stream.
	 * @param[out] root Receives the new node. It is left unchanged at an end marker, at
	 *        the end of the file, when the line is not recognised and when the header
	 *        of a project or unit cannot be read.
	 */
	void loadWorkflow(std::ifstream& file, WorkNode*& root);
public:
	/** @brief Destroys the file system and every memento it holds. */
	~FileSystem();

	/**
	 * @brief Stores a memento in memory under a name.
	 *
	 * If a different memento is already stored under that name, it is deleted and
	 * replaced. Storing the same memento under the same name again does nothing.
	 *
	 * @param name Key to store it under.
	 * @param memento The memento to store. Ownership passes to this object.
	 */
	void saveMemento(std::string name, Memento* memento);

	/**
	 * @brief Gets a stored memento.
	 * @param name The name it was stored under.
	 * @return The memento, or nullptr if there is none with that name. The FileSystem still owns it.
	 */
	Memento* loadMemento(std::string name);

	/**
	 * @brief Writes a stored memento to a file.
	 *
	 * The file is `saves/` followed by the file name and `.tvd`. The `saves` folder
	 * must already exist. A message is printed to the error stream, and nothing is
	 * written, if no memento (or an empty memento) is stored under the name or if the
	 * file cannot be opened.
	 *
	 * @param name Name of the stored memento.
	 * @param filename Name of the file without folder or extension, for example "studio".
	 */
	void saveToFile(std::string name, std::string filename);

	/**
	 * @brief Reads a file and stores the result as a memento.
	 *
	 * Reads `saves/` followed by the file name and `.tvd`, and stores the tree that
	 * was read under the given name (replacing any memento with that name). A message
	 * is printed to the error stream, and nothing is stored, if the file cannot be
	 * opened, is empty, does not start with `Company` or `Workflow`, or no tree can be
	 * read from it.
	 *
	 * @param name Name to store the loaded memento under.
	 * @param filename Name of the file without folder or extension, for example "studio".
	 */
	void loadFromFile(std::string name, std::string filename);
};

#endif
