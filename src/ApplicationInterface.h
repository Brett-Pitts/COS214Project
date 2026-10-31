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
 * Gives one entry point for the two demo scenarios, for replacing the company and
 * the workflow, and for taking and restoring snapshots. The four functions that are
 * meant to build a company and a workflow interactively (createProject(),
 * addSubcomponent(), addDepartment() and addEmployee()) are not implemented yet and
 * throw.
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
	WorkNode* workflowRoot;   ///< Root of the work tree (project), or nullptr if none. Owned.
	CompanyNode* companyRoot; ///< Root of the company tree, or nullptr if none. Owned.

public:
	/** @brief Creates an interface with no company and no workflow loaded. */
	ApplicationInterface() : workflowRoot(nullptr), companyRoot(nullptr) {}

	/**
	 * @brief Not implemented yet: always throws.
	 * @throws const char* The text "Not yet implemented".
	 */
	void createProject();

	/**
	 * @brief Not implemented yet: always throws.
	 * @throws const char* The text "Not yet implemented".
	 */
	void addSubcomponent();

	/**
	 * @brief Not implemented yet: always throws.
	 * @throws const char* The text "Not yet implemented".
	 */
	void addDepartment();

	/**
	 * @brief Not implemented yet: always throws.
	 * @throws const char* The text "Not yet implemented".
	 */
	void addEmployee();

	/**
	 * @brief Builds the first demo scenario (game studio) and makes it the current workflow and company.
	 *
	 * The workflow is a project called "Fortnite" with the phases Preparation,
	 * Creation, Marketing, QATesting, Servers and Launch, linked by prerequisites. The
	 * company has development, business and operations departments and a management
	 * team, with an intern, a junior and a senior for each of the professions Coder,
	 * Artist, Marketing, HR, QA, IT and Manager. The professions are registered.
	 *
	 * The new trees replace the current ones without deleting them, so call this on a
	 * new interface. All work nodes are left in WaitingForPrerequisites and no work
	 * mediator is attached to them.
	 */
	void createScenarioA();

	/**
	 * @brief Builds the second demo scenario (house construction) and makes it the current workflow and company.
	 *
	 * The workflow is a project called "House" with the phases Design, Structure,
	 * Services, Inspection and Handover, linked by prerequisites. The company has
	 * design and construction departments, a Manager team with one senior site
	 * manager, and an Inspector team (the municipal inspection). Every other team has
	 * an intern, a junior and a senior. The professions are Architect, Engineer,
	 * Bricklayer, Plumber, Electrician, Manager and Inspector, registered if needed.
	 *
	 * The new trees replace the current ones without deleting them, so call this on a
	 * new interface. All work nodes are left in WaitingForPrerequisites and no work
	 * mediator is attached to them.
	 */
	void createScenarioB();

	/**
	 * @brief Replaces the current workflow.
	 *
	 * The old workflow is deleted first, unless it is the same pointer.
	 *
	 * @param root New root of the work tree. Ownership passes to this object.
	 */
	void setWorkflow(WorkNode* root);

	/**
	 * @brief Replaces the current company.
	 *
	 * The old company is deleted first, unless it is the same pointer.
	 *
	 * @param root New root of the company tree. Ownership passes to this object.
	 */
	void setCompany(CompanyNode* root);

	/**
	 * @brief Captures a snapshot of the company or the workflow.
	 *
	 * The memento holds a copy made with clone() of the selected tree. A copy of the
	 * workflow keeps the names, retry limits, required professions and structure, but
	 * not the states or the prerequisites (see WorkNode::clone()).
	 *
	 * @param type Selects whether the company or the workflow is saved.
	 * @return A newly allocated Memento (the caller owns it), or nullptr if that tree
	 *         has not been set or the type is not recognised.
	 */
	Memento* createMemento(MementoType type);

	/**
	 * @brief Restores the company or the workflow from a snapshot.
	 *
	 * The current tree of the same kind is deleted and replaced by a copy (clone())
	 * of the memento's tree, so the memento is not changed and can be used again. The
	 * type of the memento decides which tree is replaced.
	 *
	 * @param memento The snapshot to restore. It must not be null (this is not checked).
	 */
	void setMemento(Memento* memento);

	/** @brief Destroys the interface and deletes the company and the workflow. */
	~ApplicationInterface();
};

#endif
