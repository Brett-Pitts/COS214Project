#include "ApplicationInterface.h"
#include "Memento.h"
#include "Team.h"
#include "Department.h"
#include "Project.h"
#include "WorkUnit.h"
#include "ConcreteEmployeeBuilder.h"
#include "EmployeeCompetency.h"
#include "EmployeeProfession.h"
#include "EmployeeDetail.h"
#include "TeamIterator.h"
#include "WorkIterator.h"
#include <algorithm>
#include "Manager.h"
#include "CompetencyLevel.h"

void ApplicationInterface::setWorkflow(WorkNode* root) {
	this->workflowRoot = root;
	assignments.clear();
	assignedTeams.clear();
	workflowLoaded = (root != nullptr);
	refreshWorkflow();
}
void ApplicationInterface::setCompany(CompanyNode* root) {
	this->companyRoot = root;
	assignedTeams.clear();
	companyLoaded = (root != nullptr);
}

static EmployeeComponent* makeEmployee(const std::string& name,
	const std::string& profession,
	const CompetencyLevel competency,
	bool isManager = false) {
	ConcreteEmployeeBuilder builder;
	builder.reset();
	builder.addProfession(profession);
	builder.addCompetency(EmployeeCompetency::getCompetencyName(competency));
	return builder.finish(name, isManager);
}

// Builds an empty team called `name` for one profession.
static Team* makeTeam(const std::string& name, const std::string& profession) {
	Team* team = new Team(name, profession);
	team->setName(name);
	return team;
}

WorkNode* createWorkflowA() {
	WorkNode* root = new Project(3, "Fortnite");


	WorkNode* Preparation = new Project(3, "Preparation");
	root->add(Preparation);

	WorkNode* Creation = new Project(3, "Creation");
	Creation->addPrerequisite("Preparation");
	root->add(Creation);

	WorkNode* Marketing = new Project(3, "Marketing");
	Marketing->addPrerequisite("Creation");
	root->add(Marketing);

	WorkNode* QATesting = new Project(3, "QATesting");
	QATesting->addPrerequisite("Creation");
	root->add(QATesting);

	WorkNode* Servers = new Project(3, "Servers");
	Servers->addPrerequisite("Creation");
	root->add(Servers);

	WorkNode* Launch = new Project(3, "Launch");
	Launch->addPrerequisite("Servers");
	Launch->addPrerequisite("Marketing");
	Launch->addPrerequisite("QATesting");
	root->add(Launch);


	//Preparation

	WorkNode* research = new WorkUnit("Market Research", "Marketing");
	Preparation->add(research);


	WorkNode* ideaGeneration = new WorkUnit("Idea Generation", "Marketing");
	Preparation->add(ideaGeneration);
	ideaGeneration->addPrerequisite(research->getName());

	WorkNode* budget = new WorkUnit("Budget", "Manager");
	Preparation->add(budget);
	budget->addPrerequisite(ideaGeneration->getName());

	WorkNode* hiring = new WorkUnit("Hiring", "HR");
	Preparation->add(hiring);
	hiring->addPrerequisite(budget->getName());

	// Creation

	WorkNode* coding = new Project(3, "Development");
	Creation->add(coding);
	WorkNode* movement = new WorkUnit("Player Movement", "Coder");
	WorkNode* world = new WorkUnit("World Creation", "Coder");
	WorkNode* story = new WorkUnit("Story", "Artist");
	coding->add(movement);
	coding->add(world);
	coding->add(story);


	WorkNode* art = new Project(3, "Art");
	Creation->add(art);

	WorkNode* conceptArt = new WorkUnit("Concept Art", "Artist");
	WorkNode* finalArt = new WorkUnit("Final Art", "Artist");
	finalArt->addPrerequisite(conceptArt->getName());

	art->add(conceptArt);
	art->add(finalArt);



	// Marketing


	WorkNode* trailer = new WorkUnit("Trailer", "Marketing");
	WorkNode* showcase = new WorkUnit("Gameplay Showcase", "Marketing");
	WorkNode* interviews = new WorkUnit("Interviews", "Marketing");
	WorkNode* ads = new WorkUnit("Ads", "Marketing");
	Marketing->add(trailer);
	Marketing->add(showcase);
	Marketing->add(interviews);
	Marketing->add(ads);

	//QATesting
	WorkNode* buildTest = new WorkUnit("Build Testing", "QA");
	QATesting->add(buildTest);

	//Servers

	const int NUM_SERVERS = 3;
	for (int i = 1; i <= NUM_SERVERS; i++) {
		std::string n = std::to_string(i);

		WorkNode* server = new Project(3, "Server " + n);
		Servers->add(server);

		WorkNode* acquire = new WorkUnit("Acquire Server " + n + " (Buy/Rent/Reallocate)", "IT");
		WorkNode* setup = new WorkUnit("Set Up Server " + n, "IT");
		setup->addPrerequisite(acquire->getName());

		server->add(acquire);
		server->add(setup);
	}
	//Launch

	WorkNode* launchGame = new WorkUnit("Launch Game", "Marketing");
	Launch->add(launchGame);

	return root;
}


CompanyNode* createCompanyA() {

	// Professions must be registered before EmployeeProfession is created with them.
	// These match the resource names used in createScenarioA().
	EmployeeProfession::registerProfession("Coder");
	EmployeeProfession::registerProfession("Artist");
	EmployeeProfession::registerProfession("Marketing");
	EmployeeProfession::registerProfession("HR");
	EmployeeProfession::registerProfession("QA");
	EmployeeProfession::registerProfession("IT");
	EmployeeProfession::registerProfession("Manager");

	// ASSUMPTION: competency names are INTERN, JUNIOR and SENIOR
	// (see EmployeeCompetency::getCompetencyLevel). Change to match your implementation.

	Department* company = new Department();
	company->setName("Game Studio");

	// ---- Development department: Coders and Artists ----
	Department* development = new Department();
	development->setName("Development");

	Team* coders = makeTeam("Coders", "Coder");
	coders->addMember(makeEmployee("Carla", "Coder", INTERN));
	coders->addMember(makeEmployee("Bob", "Coder", JUNIOR));
	coders->addMember(makeEmployee("Alice", "Coder", SENIOR));
	development->add(coders);

	Team* artists = makeTeam("Artists", "Artist");
	artists->addMember(makeEmployee("Farid", "Artist", INTERN));
	artists->addMember(makeEmployee("Erin", "Artist", JUNIOR));
	artists->addMember(makeEmployee("Dan", "Artist", SENIOR));
	development->add(artists);

	company->add(development);

	// ---- Business department: Marketing, HR ----
	Department* business = new Department();
	business->setName("Business");

	Team* marketing = makeTeam("Marketing", "Marketing");
	marketing->addMember(makeEmployee("Ivy", "Marketing", INTERN));
	marketing->addMember(makeEmployee("Henry", "Marketing", JUNIOR));
	marketing->addMember(makeEmployee("Gina", "Marketing", SENIOR));
	business->add(marketing);

	Team* hr = makeTeam("HR", "HR");
	hr->addMember(makeEmployee("Mira", "HR", INTERN));
	hr->addMember(makeEmployee("Kira", "HR", JUNIOR));
	hr->addMember(makeEmployee("Jack", "HR", SENIOR));
	business->add(hr);

	company->add(business);

	// ---- Operations department: QA, IT/Servers ----
	Department* operations = new Department();
	operations->setName("Operations");

	Team* qa = makeTeam("QA", "QA");
	qa->addMember(makeEmployee("Noah", "QA", INTERN));
	qa->addMember(makeEmployee("Maya", "QA", JUNIOR));
	qa->addMember(makeEmployee("Liam", "QA", SENIOR));
	operations->add(qa);

	Team* it = makeTeam("IT", "IT");
	it->addMember(makeEmployee("Sam", "IT", INTERN));
	it->addMember(makeEmployee("Pedro", "IT", JUNIOR));
	it->addMember(makeEmployee("Olivia", "IT", SENIOR));
	operations->add(it);

	company->add(operations);

	// ---- Management: managers handle escalations and can cancel projects ----
	Team* managers = makeTeam("Managers", "Manager");
	managers->addMember(makeEmployee("Tara", "Manager", INTERN, true));
	managers->addMember(makeEmployee("Rosa", "Manager", JUNIOR, true));
	managers->addMember(makeEmployee("Quinn", "Manager", SENIOR, true));
	company->add(managers);


	return company;
}


void ApplicationInterface::createScenarioA() {
	delete workflowRoot;
	delete companyRoot;
	workflowRoot = createWorkflowA();
	companyRoot = createCompanyA();
	assignments.clear();
	assignedTeams.clear();
	workflowLoaded = true;
	companyLoaded = true;
	refreshWorkflow();
}

// Scenario B: a building contractor builds one house.
// Uses the same helpers (makeTeam / makeEmployee) as scenario A.

WorkNode* createWorkflowB() {
	WorkNode* root = new Project(3, "House");

	WorkNode* Design = new Project(3, "Design");
	root->add(Design);

	WorkNode* Structure = new Project(3, "Structure");
	Structure->addPrerequisite("Design");          // needs the plans
	root->add(Structure);

	WorkNode* Services = new Project(3, "Services");
	Services->addPrerequisite("Structure");
	root->add(Services);

	WorkNode* Inspection = new Project(3, "Inspection");
	Inspection->addPrerequisite("Services");
	root->add(Inspection);

	WorkNode* Handover = new Project(3, "Handover");
	Handover->addPrerequisite("Inspection");
	root->add(Handover);


	// Design: site survey (engineer), then plans (architect)

	WorkNode* survey = new WorkUnit("Site Survey", "Engineer");
	Design->add(survey);

	WorkNode* plans = new WorkUnit("Plans", "Architect");
	plans->addPrerequisite(survey->getName());
	Design->add(plans);


	// Structure: foundations, then framing (bricklayer)

	WorkNode* foundations = new WorkUnit("Foundations", "Bricklayer");
	Structure->add(foundations);

	WorkNode* framing = new WorkUnit("Framing", "Bricklayer");
	framing->addPrerequisite(foundations->getName());
	Structure->add(framing);


	// Services: plumbing and electrical run in parallel (no prerequisites between them)

	WorkNode* plumbing = new WorkUnit("Plumbing", "Plumber");
	WorkNode* electrical = new WorkUnit("Electrical", "Electrician");
	Services->add(plumbing);
	Services->add(electrical);


	// Inspection: municipal inspector (stub)

	WorkNode* municipalInspection = new WorkUnit("Municipal Inspection", "Inspector");
	Inspection->add(municipalInspection);


	// Handover (architect)

	WorkNode* handoverUnit = new WorkUnit("Handover", "Architect");
	Handover->add(handoverUnit);

	return root;
}


CompanyNode* createCompanyB() {


	const char* professions[] = { "Architect", "Engineer", "Bricklayer", "Plumber",
								  "Electrician", "Manager", "Inspector" };
	for (const char* p : professions) {
		if (!EmployeeProfession::hasProfession(p)) {
			EmployeeProfession::registerProfession(p);
		}
	}

	Department* company = new Department();
	company->setName("Building Contractor");

	// ---- Design department: Architects and Engineers ----
	Department* design = new Department();
	design->setName("Design");

	Team* architects = makeTeam("Architects", "Architect");
	architects->addMember(makeEmployee("Zara", "Architect", INTERN));
	architects->addMember(makeEmployee("Yusuf", "Architect", JUNIOR));
	architects->addMember(makeEmployee("Willa", "Architect", SENIOR));
	design->add(architects);

	Team* engineers = makeTeam("Engineers", "Engineer");
	engineers->addMember(makeEmployee("Victor", "Engineer", INTERN));
	engineers->addMember(makeEmployee("Uma", "Engineer", JUNIOR));
	engineers->addMember(makeEmployee("Thabo", "Engineer", SENIOR));
	design->add(engineers);

	company->add(design);

	// ---- Construction department: Bricklayers, Plumbers, Electricians ----
	Department* construction = new Department();
	construction->setName("Construction");

	Team* bricklayers = makeTeam("Bricklayers", "Bricklayer");
	bricklayers->addMember(makeEmployee("Sipho", "Bricklayer", INTERN));
	bricklayers->addMember(makeEmployee("Ruben", "Bricklayer", JUNIOR));
	bricklayers->addMember(makeEmployee("Pieter", "Bricklayer", SENIOR));
	construction->add(bricklayers);

	Team* plumbers = makeTeam("Plumbers", "Plumber");
	plumbers->addMember(makeEmployee("Omar", "Plumber", INTERN));
	plumbers->addMember(makeEmployee("Nandi", "Plumber", JUNIOR));
	plumbers->addMember(makeEmployee("Moses", "Plumber", SENIOR));
	construction->add(plumbers);

	Team* electricians = makeTeam("Electricians", "Electrician");
	electricians->addMember(makeEmployee("Lerato", "Electrician", INTERN));
	electricians->addMember(makeEmployee("Kabelo", "Electrician", JUNIOR));
	electricians->addMember(makeEmployee("Johan", "Electrician", SENIOR));
	construction->add(electricians);

	company->add(construction);

	// ---- Site management: the site manager ----
	Team* managers = makeTeam("Managers", "Manager");
	managers->addMember(makeEmployee("Ingrid", "Manager", SENIOR, true));
	company->add(managers);

	// ---- Municipality: inspector (stub) ----
	Team* inspectors = makeTeam("Inspectors", "Inspector");
	inspectors->addMember(makeEmployee("Hendrik", "Inspector", INTERN));
	inspectors->addMember(makeEmployee("Gugu", "Inspector", JUNIOR));
	inspectors->addMember(makeEmployee("Frans", "Inspector", SENIOR));
	company->add(inspectors);

	return company;
}

void ApplicationInterface::createScenarioB() {
	delete workflowRoot;
	delete companyRoot;
	workflowRoot = createWorkflowB();
	companyRoot = createCompanyB();
	assignments.clear();
	assignedTeams.clear();
	workflowLoaded = true;
	companyLoaded = true;
	refreshWorkflow();
}

Memento* ApplicationInterface::createMemento(MementoType type) {
	switch (type) {
	case MementoType::company:
		if (companyRoot == nullptr) {
			return nullptr;
		}
		return new Memento(companyRoot->clone(), MementoType::company);
	case MementoType::workflow:
		if (workflowRoot == nullptr) {
			return nullptr;
		}
		return new Memento(workflowRoot->clone(), MementoType::workflow);
	default:
		return nullptr;
	}
}

void ApplicationInterface::setMemento(Memento* memento) {
	if (memento == nullptr || memento->getRoot() == nullptr) {
		return;
	}
	switch (memento->getType()) {
	case MementoType::company:
	{
		CompanyNode* restored = ((CompanyNode*)memento->getRoot())->clone();
		delete companyRoot;
		companyRoot = restored;
		assignedTeams.clear();
		for (const auto& entry : assignments) {
			for (Team* team : getTeams()) {
				if (team->getName() == entry.second) {
					assignedTeams[entry.first] = team;
					break;
				}
			}
		}
		break;
	}
	case MementoType::workflow:
	{
		WorkNode* restored = ((WorkNode*)memento->getRoot())->clone();
		delete workflowRoot;
		workflowRoot = restored;
		assignments.clear();
		assignedTeams.clear();
		refreshWorkflow();
		break;
	}
	default:
		break;
	}
}

// for gui:
bool ApplicationInterface::saveToFile(MementoType type, const std::string& path) {
	Memento* memento = createMemento(type);
	if (memento == nullptr) {
		return false;
	}
	const std::string key = (type == MementoType::company) ? "company" : "workflow";
	fileSystem.saveMemento(key, memento);
	return fileSystem.saveToFile(key, path);
}

// for gui:
bool ApplicationInterface::loadFromFile(MementoType type, const std::string& path) {
	const std::string key = "loaded";
	if (!fileSystem.loadFromFile(key, path)) {
		return false;
	}
	Memento* memento = fileSystem.loadMemento(key);
	if (memento == nullptr || memento->getType() != type) {
		return false;
	}
	setMemento(memento);
	if (type == MementoType::company) {
		companyLoaded = true;
	}
	else {
		workflowLoaded = true;
	}
	return true;
}

// for gui:
bool ApplicationInterface::isLoaded(MementoType type) const {
	if (type == MementoType::company) {
		return companyLoaded;
	}
	if (type == MementoType::workflow) {
		return workflowLoaded;
	}
	return false;
}

// for gui:
CompanyNode* ApplicationInterface::getCompany() {
	if (companyRoot == nullptr) {
		Department* root = new Department("Company");
		companyRoot = root;
	}
	return companyRoot;
}

// for gui:
Department* ApplicationInterface::addDepartment(CompanyNode* parent, const std::string& name) {
	if (parent == nullptr) {
		parent = getCompany();
	}
	if (dynamic_cast<Team*>(parent) != nullptr) {
		return nullptr;
	}
	Department* department = new Department();
	department->setName(name);
	parent->add(department);
	return department;
}

// for gui:
Team* ApplicationInterface::addTeam(CompanyNode* parent, const std::string& name, const std::string& profession) {
	if (parent == nullptr) {
		parent = getCompany();
	}
	if (dynamic_cast<Team*>(parent) != nullptr) {
		return nullptr;
	}
	EmployeeProfession::registerProfession(profession);
	Team* team = new Team(name, profession);
	team->setName(name);
	parent->add(team);
	return team;
}

// for gui:
bool ApplicationInterface::addEmployee(Team* team, const std::string& name, const std::string& profession,
	const std::string& competency, bool isManager) {
	if (team == nullptr || name.empty()) {
		return false;
	}
	ConcreteEmployeeBuilder builder;
	builder.reset();
	if (!profession.empty()) {
		builder.addProfession(profession);
	}
	if (!competency.empty() && !builder.addCompetency(competency)) {
		builder.reset();
		return false;
	}
	EmployeeComponent* employee = builder.finish(name, isManager);
	if (!team->addMember(employee)) {
		delete employee;
		return false;
	}
	return true;
}

// for gui:
bool ApplicationInterface::setCompetency(Team* team, int memberIndex, const std::string& competency) {
	if (team == nullptr || EmployeeCompetency::getCompetencyLevel(competency) < 0) {
		return false;
	}
	EmployeeComponent* old = team->getMember(memberIndex);
	if (old == nullptr) {
		return false;
	}
	std::string profession = team->getProfession();
	if (profession.empty()) {
		for (const std::string& p : getProfessions()) {
			if (old->compareProfession(p)) {
				profession = p;
				break;
			}
		}
	}
	ConcreteEmployeeBuilder builder;
	builder.reset();
	if (!profession.empty()) {
		builder.addProfession(profession);
	}
	builder.addCompetency(competency);
	EmployeeComponent* replacement = builder.finish(old->getName(), old->getCore()->getKind() == "Manager");
	if (!team->replaceMember(memberIndex, replacement)) {
		delete replacement;
		return false;
	}
	delete old;
	return true;
}

// for gui:
std::vector<Team*> ApplicationInterface::getTeams(const std::string& profession) {
	std::vector<Team*> teams;
	TeamIterator it(getCompany(), profession);
	for (it.first(); !it.isDone(); it.next()) {
		teams.push_back(it.currentItem());
	}
	return teams;
}

// for gui:
std::vector<std::string> ApplicationInterface::getProfessions() {
	std::vector<std::string> professions;
	for (Team* team : getTeams()) {
		std::string p = team->getProfession();
		if (!p.empty() && std::find(professions.begin(), professions.end(), p) == professions.end()) {
			professions.push_back(p);
		}
	}
	return professions;
}

// for gui:
WorkNode* ApplicationInterface::getWorkflow() {
	if (workflowRoot == nullptr) {
		workflowRoot = new Project(3, "Workflow");
		refreshWorkflow();
	}
	return workflowRoot;
}

// for gui:
std::vector<WorkNode*> ApplicationInterface::getWorkNodes() {
	std::vector<WorkNode*> nodes;
	WorkIterator it(getWorkflow(), false);
	for (it.first(); !it.isDone(); it.next()) {
		nodes.push_back(it.currentItem());
	}
	return nodes;
}

// for gui:
std::vector<WorkUnit*> ApplicationInterface::getWorkUnits() {
	std::vector<WorkUnit*> units;
	for (WorkNode* node : getWorkNodes()) {
		WorkUnit* unit = dynamic_cast<WorkUnit*>(node);
		if (unit != nullptr) {
			units.push_back(unit);
		}
	}
	return units;
}

// for gui:
void ApplicationInterface::refreshWorkflow() {
	mediator.colleagues.clear();
	WorkIterator it(workflowRoot, false);
	for (it.first(); !it.isDone(); it.next()) {
		it.currentItem()->setMediator(&mediator);
		mediator.colleagues.push_back(it.currentItem());
	}
}

// for gui:
Project* ApplicationInterface::createProject(Project* parent, const std::string& name) {
	WorkNode* root = getWorkflow();
	if (name.empty() || root->hasName(name)) {
		return nullptr;
	}
	if (parent == nullptr) {
		parent = static_cast<Project*>(root);
	}
	Project* project = new Project(3, name);
	parent->add(project);
	refreshWorkflow();
	return project;
}

// for gui:
WorkUnit* ApplicationInterface::addSubcomponent(Project* parent, const std::string& name, const std::string& profession) {
	WorkNode* root = getWorkflow();
	if (name.empty() || root->hasName(name)) {
		return nullptr;
	}
	if (parent == nullptr) {
		parent = static_cast<Project*>(root);
	}
	WorkUnit* unit = new WorkUnit(name, profession);
	parent->add(unit);
	refreshWorkflow();
	return unit;
}

// for gui:
bool ApplicationInterface::renameWorkNode(WorkNode* node, const std::string& name) {
	if (node == nullptr || name.empty()) {
		return false;
	}
	if (node->getName() == name) {
		return true;
	}
	if (getWorkflow()->hasName(name)) {
		return false;
	}
	std::string oldName = node->getName();
	node->setName(name);
	for (WorkNode* other : getWorkNodes()) {
		const std::vector<std::string> prereqs = other->getPrerequisites();
		if (std::find(prereqs.begin(), prereqs.end(), oldName) != prereqs.end()) {
			other->removePrerequisite(oldName);
			other->addPrerequisite(name);
		}
	}
	auto it = assignments.find(oldName);
	if (it != assignments.end()) {
		assignments[name] = it->second;
		assignments.erase(oldName);
	}
	auto teamIt = assignedTeams.find(oldName);
	if (teamIt != assignedTeams.end()) {
		assignedTeams[name] = teamIt->second;
		assignedTeams.erase(oldName);
	}
	return true;
}

// for gui:
bool ApplicationInterface::removeWorkNode(WorkNode* node) {
	if (node == nullptr || node == workflowRoot || node->getParent() == nullptr) {
		return false;
	}
	std::vector<WorkNode*> doomed;
	WorkIterator doomedIt(node, false);
	for (doomedIt.first(); !doomedIt.isDone(); doomedIt.next()) {
		doomed.push_back(doomedIt.currentItem());
	}

	node->getParent()->remove(node);

	std::vector<WorkNode*> released;
	for (WorkNode* gone : doomed) {
		std::string goneName = gone->getName();
		assignments.erase(goneName);
		assignedTeams.erase(goneName);
		for (WorkNode* other : getWorkNodes()) {
			const std::vector<std::string>& prereqs = other->getPrerequisites();
			if (std::find(prereqs.begin(), prereqs.end(), goneName) != prereqs.end()) {
				other->removePrerequisite(goneName);
				released.push_back(other);
			}
		}
	}
	delete node;
	refreshWorkflow();
	for (WorkNode* other : released) {
		if (other->getPrerequisites().empty() && other->getStateName() == "WaitingForPrerequisites") {
			other->advanceState();
		}
	}
	return true;
}

// for gui:
WorkNode* ApplicationInterface::findWorkNode(const std::string& name) {
	for (WorkNode* node : getWorkNodes()) {
		if (node->getName() == name) {
			return node;
		}
	}
	return nullptr;
}

// for gui:
bool ApplicationInterface::dependsOn(WorkNode* from, const std::string& name, int depth) {
	if (from == nullptr || depth > 1000) {
		return false;
	}
	for (const std::string& prereq : from->getPrerequisites()) {
		if (prereq == name) {
			return true;
		}
		if (dependsOn(findWorkNode(prereq), name, depth + 1)) {
			return true;
		}
	}
	return false;
}

// for gui:
bool ApplicationInterface::addPrerequisite(WorkNode* node, const std::string& name) {
	if (node == nullptr || name.empty() || node->getName() == name) {
		return false;
	}
	WorkNode* target = findWorkNode(name);
	if (target == nullptr || target->isCompleted()) {
		return false;
	}
	const std::vector<std::string>& existing = node->getPrerequisites();
	if (std::find(existing.begin(), existing.end(), name) != existing.end()) {
		return false;
	}
	for (WorkNode* up = node->getParent(); up != nullptr; up = up->getParent()) {
		if (up == target) {
			return false;
		}
	}
	if (dependsOn(target, node->getName(), 0)) {
		return false;
	}
	node->addPrerequisite(name);
	if (node->isReady()) {
		node->setState("WaitingForPrerequisites");
	}
	return true;
}

// for gui:
bool ApplicationInterface::removePrerequisite(WorkNode* node, const std::string& name) {
	if (node == nullptr) {
		return false;
	}
	const std::vector<std::string>& existing = node->getPrerequisites();
	if (std::find(existing.begin(), existing.end(), name) == existing.end()) {
		return false;
	}
	node->removePrerequisite(name);
	if (node->getPrerequisites().empty() && node->getStateName() == "WaitingForPrerequisites") {
		node->advanceState();
	}
	return true;
}

// for gui:
std::string ApplicationInterface::checkAssignable(WorkUnit* unit, Team* team) {
	if (unit == nullptr || team == nullptr) {
		return "Nothing to assign.";
	}
	std::string state = unit->getStateName();
	if (state == "Completed" || state == "Cancelled" || state == "Failed") {
	return unit->getName() + " is already " + state + ".";
	}
	std::string required = unit->getRequiredProfession();
	if (!required.empty() && !team->getProfession().empty() && required != team->getProfession()) {
		return unit->getName() + " needs a " + required + " team, not " + team->getName() + ".";
	}
	if (!required.empty() && !EmployeeProfession::hasProfession(required)) {
		return "Nobody in the company has the profession " + required + ".";
	}
	if (state == "WaitingForPrerequisites") {
		if (unit->isWaitingForPrerequisites()) {
			return unit->getName() + " is still waiting for its prerequisites.";
		}
		unit->advanceState();
	}
	return "";
}

// for gui:
void ApplicationInterface::recordAssignment(WorkUnit* unit, Team* team) {
	assignments[unit->getName()] = team->getName();
	assignedTeams[unit->getName()] = team;
}

// for gui:
Team* ApplicationInterface::teamFor(const std::string& unitName) {
	std::map<std::string, Team*>::iterator found = assignedTeams.find(unitName);
	if (found == assignedTeams.end()) {
		return nullptr;
	}
	for (Team* team : getTeams()) {
		if (team == found->second) {
			return team;
		}
	}
	return nullptr;
}

// for gui:
std::string ApplicationInterface::assignWork(WorkUnit* unit, Team* team) {
	std::string problem = checkAssignable(unit, team);
	if (!problem.empty()) {
		return problem;
	}

	team->assignWork(unit);
	recordAssignment(unit, team);
	return unit->getName() + " is now " + unit->getStateName() + ".";
}

// for gui:
std::string ApplicationInterface::assignOnly(WorkUnit* unit, Team* team) {
	std::string problem = checkAssignable(unit, team);
	if (!problem.empty()) {
		return problem;
	}

	recordAssignment(unit, team);
	return unit->getName() + " is assigned to " + team->getName() + " and is " + unit->getStateName() + ".";
}

// for gui:
std::string ApplicationInterface::checkSteppable(WorkUnit* unit, Team*& team) {
	if (unit == nullptr) {
		return "Nothing to do.";
	}
	std::string name = unit->getName();
	std::string state = unit->getStateName();
	if (state == "Completed" || state == "Cancelled" || state == "Failed") {
		return name + " is already " + state + ".";
	}
	if (state == "WaitingForPrerequisites") {
		return name + " is still waiting for its prerequisites.";
	}
	team = teamFor(name);
	if (team == nullptr) {
		return name + " is not assigned to a team that still exists. Assign it again.";
	}
	return "";
}

// for gui:
std::string ApplicationInterface::retryOrFailText(WorkUnit* unit, Team* team, const std::string& cause) {
	std::string name = unit->getName();
	if (!team->retryOrFail(unit)) {
		return "Nobody in " + team->getName() + " can take " + name + ".";
	}
	if (unit->getStateName() == "Failed") {
		return name + " failed (" + cause + ") and it has no retries left.";
	}
	return name + " failed (" + cause + "), so it was sent back to Ready for another try.";
}

// for gui:
std::string ApplicationInterface::stepWork(WorkUnit* unit) {
	Team* team = nullptr;
	std::string problem = checkSteppable(unit, team);
	if (!problem.empty()) {
		return problem;
	}
	std::string before = unit->getStateName();
	if (team->stepWork(unit)) {
		return unit->getName() + " moved from " + before + " to " + unit->getStateName() + ".";
	}
	return retryOrFailText(unit, team, "nobody in " + team->getName() + " could move it on from " + before);
}

// for gui:
std::string ApplicationInterface::failWork(WorkUnit* unit) {
	Team* team = nullptr;
	std::string problem = checkSteppable(unit, team);
	if (!problem.empty()) {
		return problem;
	}
	return retryOrFailText(unit, team, "it was forced to fail");
}

// for gui:
std::string ApplicationInterface::forceState(WorkUnit* unit, const std::string& stateName) {
	if (unit == nullptr) {
		return "Nothing to change.";
	}
	const char* known[] = { "WaitingForPrerequisites", "Ready", "Ongoing", "Escalated", "Completed", "Cancelled", "Failed" };
	bool found = false;
	for (const char* name : known) {
		if (stateName == name) {
			found = true;
		}
	}
	if (!found) {
		return stateName + " is not a state.";
	}
	if (unit->getStateName() == stateName) {
		return unit->getName() + " is already " + stateName + ".";
	}
	if (stateName == "Completed") {
		unit->setState("Escalated");
		unit->advanceState();
	}
	else {
		unit->setState(stateName);
	}
	return unit->getName() + " is now " + unit->getStateName() + ".";
}

// for gui:
std::string ApplicationInterface::cancelWork(WorkUnit* unit) {
	if (unit == nullptr) {
		return "Nothing to cancel.";
	}
	std::string state = unit->getStateName();
	if (state == "Completed" || state == "Cancelled" || state == "Failed") {
		return unit->getName() + " is already " + state + ".";
	}
	unit->setState("Cancelled");
	return unit->getName() + " was cancelled.";
}

// for gui:
void ApplicationInterface::unassign(const std::string& unitName) {
	assignments.erase(unitName);
	assignedTeams.erase(unitName);
}

const std::map<std::string, std::string>& ApplicationInterface::getAssignments() const {
	return assignments;
}

ApplicationInterface::~ApplicationInterface() {
	delete workflowRoot;
	delete companyRoot;
}
