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

class ApplicationInterface {

private:
	WorkNode* workflowRoot;
	CompanyNode* companyRoot;

	// for gui:
	FileSystem fileSystem;
	ConcreteWorkMediator mediator;
	std::map<std::string, std::string> assignments;
	std::map<std::string, Team*> assignedTeams;
	bool companyLoaded;
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
	ApplicationInterface() : workflowRoot(nullptr), companyRoot(nullptr), companyLoaded(false), workflowLoaded(false) {}

	void setWorkflow(WorkNode* root);
	void setCompany(CompanyNode* root);
	Memento* createMemento(MementoType type);
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

	~ApplicationInterface();
};

#endif
