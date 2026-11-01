#ifndef APPLICATIONINTERFACE_H
#define APPLICATIONINTERFACE_H

#include <map>
#include <string>
#include <vector>
#include "WorkNode.h"
#include "CompanyNode.h"
#include "MementoType.h"
#include "FileSystem.h"
#include "ConcreteWorkMediator.h"

class Memento;
class Team;
class WorkUnit;
class Project;
class Department;

/** @brief Entry point of the system. Holds the current workflow and company. */
class ApplicationInterface {

private:
	/** Root of the current workflow. Owned. */
	WorkNode* workflowRoot;
	/** Root of the current company. Owned. */
	CompanyNode* companyRoot;

	// for gui:
	FileSystem fileSystem;
	/** Mediator for the work nodes. */
	ConcreteWorkMediator mediator;
	/** Name of the team each unit is assigned to, by unit name. */
	std::map<std::string, std::string> assignments;
	/** The same assignments as team pointers. */
	std::map<std::string, Team*> assignedTeams;
	/** True once a company is set. */
	bool companyLoaded;
	/** True once a workflow is set. */
	bool workflowLoaded;

	// for gui:
	WorkNode* findWorkNode(const std::string& name);
	bool dependsOn(WorkNode* from, const std::string& name, int depth);
	void refreshWorkflow();
	std::string checkAssignable(WorkUnit* unit, Team* team);
	void recordAssignment(WorkUnit* unit, Team* team);
	Team* teamFor(const std::string& unitName);
	std::string checkSteppable(WorkUnit* unit, Team*& team);
	std::string retryOrFailText(WorkUnit* unit, Team* team, const std::string& cause);

public:
	/** @brief Creates an interface with no workflow or company. */
	ApplicationInterface() : workflowRoot(nullptr), companyRoot(nullptr),mediator(ConcreteWorkMediator()), companyLoaded(false), workflowLoaded(false) {}

	/** @brief Sets the current workflow. The old one is not deleted. */
	void setWorkflow(WorkNode* root);
	/** @brief Sets the current company. The old one is not deleted. */
	void setCompany(CompanyNode* root);

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
	/** @brief Saves a copy of the company or workflow in a new memento. nullptr if there is none. */
	Memento* createMemento(MementoType type);
	/** @brief Restores the company or workflow from a memento. The current one is deleted. */
	void setMemento(Memento* memento);

	// for gui:
	bool saveToFile(MementoType type, const std::string& path);
	bool loadFromFile(MementoType type, const std::string& path);
	bool isLoaded(MementoType type) const;

	// for gui:
	CompanyNode* getCompany();
	Department* addDepartment(CompanyNode* parent, const std::string& name);
	Team* addTeam(CompanyNode* parent, const std::string& name, const std::string& profession);
	bool addEmployee(Team* team, const std::string& name, const std::string& profession,
		const std::string& competency, bool isManager);
	bool setCompetency(Team* team, int memberIndex, const std::string& competency);
	std::vector<Team*> getTeams(const std::string& profession = "");
	std::vector<std::string> getProfessions();

	// for gui:
	WorkNode* getWorkflow();
	Project* createProject(Project* parent, const std::string& name);
	WorkUnit* addSubcomponent(Project* parent, const std::string& name, const std::string& profession);
	bool renameWorkNode(WorkNode* node, const std::string& name);
	bool removeWorkNode(WorkNode* node);
	bool addPrerequisite(WorkNode* node, const std::string& name);
	bool removePrerequisite(WorkNode* node, const std::string& name);
	std::vector<WorkNode*> getWorkNodes();
	std::vector<WorkUnit*> getWorkUnits();

	// for gui:
	std::string assignWork(WorkUnit* unit, Team* team);
	std::string assignOnly(WorkUnit* unit, Team* team);
	std::string stepWork(WorkUnit* unit);
	std::string cancelWork(WorkUnit* unit);
	std::string failWork(WorkUnit* unit);
	std::string forceState(WorkUnit* unit, const std::string& stateName);
	void unassign(const std::string& unitName);
	const std::map<std::string, std::string>& getAssignments() const;

	/** @brief Deletes the workflow and company. */
	~ApplicationInterface();
};

#endif
