#ifndef APPLICATIONINTERFACE_H
#define APPLICATIONINTERFACE_H

#include "WorkNode.h"
#include "CompanyNode.h"
#include "MementoType.h"
class Memento;

/**
 * @file ApplicationInterface.h
 * @brief Declares the single entry point used by the user interface.
 */

/**
 * @class ApplicationInterface
 * @brief Facade over the TIVIDY engine: owns the company and workflow trees.
 *
 * Presents one simple interface for building a company, building projects,
 * running the demo scenarios and saving or restoring state.
 *
 * @par Design patterns
 * - Facade: the one entry point for the interface.
 * - Memento (Originator): creates and restores Memento objects that hold a
 *   copy of the company or the workflow. FileSystem is the caretaker.
 *
 * @see Memento, FileSystem, WorkNode, CompanyNode
 */
class ApplicationInterface {

private:
	WorkNode* workflowRoot;   ///< Root of the work tree (project), or nullptr if none.
	CompanyNode* companyRoot; ///< Root of the company tree, or nullptr if none.

public:
	/** @brief Creates an interface with no company and no workflow loaded. */
	ApplicationInterface() : workflowRoot(nullptr), companyRoot(nullptr) {}

	/** @brief Creates a new project and makes it (or attaches it as) part of the workflow. */
	void createProject();

	/** @brief Adds a sub-project or work unit to a project in the workflow. */
	void addSubcomponent();

	/** @brief Adds a department or team to the company structure. */
	void addDepartment();

	/** @brief Builds an employee and adds it to a team in the company. */
	void addEmployee();

	/** @brief Builds and prepares the first industry scenario (game studio). */
	void createScenarioA();

	/** @brief Builds and prepares the second industry scenario (house construction). */
	void createScenarioB();

	/**
	 * @brief Replaces the current workflow.
	 * @param root New root of the work tree. Ownership passes to this object.
	 */
	void setWorkflow(WorkNode* root);

	/**
	 * @brief Replaces the current company.
	 * @param root New root of the company tree. Ownership passes to this object.
	 */
	void setCompany(CompanyNode* root);

	/**
	 * @brief Captures a snapshot of the company or the workflow.
	 * @param type Selects whether the company or the workflow is saved.
	 * @return A newly allocated Memento holding a deep copy (made with clone()).
	 */
	Memento* createMemento(MementoType type);

	/**
	 * @brief Restores the company or workflow from a snapshot.
	 * @param memento The snapshot to restore. Its type decides what is replaced.
	 */
	void setMemento(Memento* memento);

	/** @brief Destroys the interface and the trees it owns. */
	~ApplicationInterface();
};

#endif
