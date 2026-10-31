#include "ApplicationInterface.h"
#include "Memento.h"
#include "Project.h"
#include "WorkUnit.h"

#include "Department.h"
#include "Team.h"
#include "EmployeeCompetency.h"
#include "EmployeeProfession.h"
#include "Employee.h"
#include "Manager.h"
#include "EmployeeDirector.h"
#include "ConcreteEmployeeBuilder.h"
#include "ConcreteWorkMediator.h"


void ApplicationInterface::createProject() {
	// TODO - implement ApplicationInterface::createProject
	throw "Not yet implemented";
}

void ApplicationInterface::addSubcomponent() {
	// TODO - implement ApplicationInterface::addSubcomponent
	throw "Not yet implemented";
}

void ApplicationInterface::addDepartment() {
	// TODO - implement ApplicationInterface::addDepartment
	throw "Not yet implemented";
}

void ApplicationInterface::addEmployee() {
	// TODO - implement ApplicationInterface::addEmployee
	throw "Not yet implemented";
}

static EmployeeComponent* makeEmployee(const std::string& name,
	const std::string& profession,
	const std::string& competency,
	bool isManager = false) {
	EmployeeComponent* core;
	if (isManager) {
		core = new Manager(name);
	}
	else {
		core = new Employee(name);
	}

	EmployeeComponent* prof = new EmployeeProfession(profession);
	prof->wrap(core);

	EmployeeComponent* comp = new EmployeeCompetency(competency);
	comp->wrap(prof);

	return comp;
}

// Builds a team of one profession and fills it with members.
static Team* makeTeam(const std::string& profession) {
	return new Team(profession);
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
	ideaGeneration->addPrerequisite(research->getName());
	Preparation->add(ideaGeneration);

	WorkNode* budget = new WorkUnit("Budget", "Manager");
	budget->addPrerequisite(ideaGeneration->getName());
	Preparation->add(budget);

	WorkNode* hiring = new WorkUnit("Hiring", "HR");
	hiring->addPrerequisite(budget->getName());
	Preparation->add(hiring);

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

	// ASSUMPTION: competency names are "Intern", "Junior" and "Senior"
	// (see EmployeeCompetency::getCompetencyLevel). Change to match your implementation.

	CompanyNode* company = new Department();

	// ---- Development department: Coders and Artists ----
	CompanyNode* development = new Department();

	Team* coders = makeTeam("Coder");
	coders->addMember(makeEmployee("Carla", "Coder", "Intern"));
	coders->addMember(makeEmployee("Bob", "Coder", "Junior"));
	coders->addMember(makeEmployee("Alice", "Coder", "Senior"));
	development->add(coders);

	Team* artists = makeTeam("Artist");
	artists->addMember(makeEmployee("Farid", "Artist", "Intern"));
	artists->addMember(makeEmployee("Erin", "Artist", "Junior"));
	artists->addMember(makeEmployee("Dan", "Artist", "Senior"));
	development->add(artists);

	company->add(development);

	// ---- Business department: Marketing, HR ----
	CompanyNode* business = new Department();

	Team* marketing = makeTeam("Marketing");
	marketing->addMember(makeEmployee("Ivy", "Marketing", "Intern"));
	marketing->addMember(makeEmployee("Henry", "Marketing", "Junior"));
	marketing->addMember(makeEmployee("Gina", "Marketing", "Senior"));
	business->add(marketing);

	Team* hr = makeTeam("HR");
	hr->addMember(makeEmployee("Mira", "HR", "Intern"));
	hr->addMember(makeEmployee("Kira", "HR", "Junior"));
	hr->addMember(makeEmployee("Jack", "HR", "Senior"));
	business->add(hr);

	company->add(business);

	// ---- Operations department: QA, IT/Servers ----
	CompanyNode* operations = new Department();

	Team* qa = makeTeam("QA");
	qa->addMember(makeEmployee("Noah", "QA", "Intern"));
	qa->addMember(makeEmployee("Maya", "QA", "Junior"));
	qa->addMember(makeEmployee("Liam", "QA", "Senior"));
	operations->add(qa);

	Team* it = makeTeam("IT");
	it->addMember(makeEmployee("Sam", "IT", "Intern"));
	it->addMember(makeEmployee("Pedro", "IT", "Junior"));
	it->addMember(makeEmployee("Olivia", "IT", "Senior"));
	operations->add(it);

	company->add(operations);

	// ---- Management: managers handle escalations and can cancel projects ----
	Team* managers = makeTeam("Manager");
	managers->addMember(makeEmployee("Tara", "Manager", "Intern", true));
	managers->addMember(makeEmployee("Rosa", "Manager", "Junior", true));
	managers->addMember(makeEmployee("Quinn", "Manager", "Senior", true));
	company->add(managers);


	return company;
}


void ApplicationInterface::createScenarioA() {
	this->workflowRoot = createWorkflowA();

	this->companyRoot = createCompanyA();

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

	CompanyNode* company = new Department();

	// ---- Design department: Architects and Engineers ----
	CompanyNode* design = new Department();

	Team* architects = makeTeam("Architect");
	architects->addMember(makeEmployee("Zara", "Architect", "Intern"));
	architects->addMember(makeEmployee("Yusuf", "Architect", "Junior"));
	architects->addMember(makeEmployee("Willa", "Architect", "Senior"));
	design->add(architects);

	Team* engineers = makeTeam("Engineer");
	engineers->addMember(makeEmployee("Victor", "Engineer", "Intern"));
	engineers->addMember(makeEmployee("Uma", "Engineer", "Junior"));
	engineers->addMember(makeEmployee("Thabo", "Engineer", "Senior"));
	design->add(engineers);

	company->add(design);

	// ---- Construction department: Bricklayers, Plumbers, Electricians ----
	CompanyNode* construction = new Department();

	Team* bricklayers = makeTeam("Bricklayer");
	bricklayers->addMember(makeEmployee("Sipho", "Bricklayer", "Intern"));
	bricklayers->addMember(makeEmployee("Ruben", "Bricklayer", "Junior"));
	bricklayers->addMember(makeEmployee("Pieter", "Bricklayer", "Senior"));
	construction->add(bricklayers);

	Team* plumbers = makeTeam("Plumber");
	plumbers->addMember(makeEmployee("Omar", "Plumber", "Intern"));
	plumbers->addMember(makeEmployee("Nandi", "Plumber", "Junior"));
	plumbers->addMember(makeEmployee("Moses", "Plumber", "Senior"));
	construction->add(plumbers);

	Team* electricians = makeTeam("Electrician");
	electricians->addMember(makeEmployee("Lerato", "Electrician", "Intern"));
	electricians->addMember(makeEmployee("Kabelo", "Electrician", "Junior"));
	electricians->addMember(makeEmployee("Johan", "Electrician", "Senior"));
	construction->add(electricians);

	company->add(construction);

	// ---- Site management: the site manager ----
	Team* managers = makeTeam("Manager");
	managers->addMember(makeEmployee("Ingrid", "Manager", "Senior", true));
	company->add(managers);

	// ---- Municipality: inspector (stub) ----
	Team* inspectors = makeTeam("Inspector");
	inspectors->addMember(makeEmployee("Hendrik", "Inspector", "Intern"));
	inspectors->addMember(makeEmployee("Gugu", "Inspector", "Junior"));
	inspectors->addMember(makeEmployee("Frans", "Inspector", "Senior"));
	company->add(inspectors);

	return company;
}

void ApplicationInterface::createScenarioB() {
	this->workflowRoot = createWorkflowB();

	this->companyRoot = createCompanyB();



}

void ApplicationInterface::setWorkflow(WorkNode* root) {
	if (workflowRoot != root) {
		delete workflowRoot;
		workflowRoot = root;
	}
}
void ApplicationInterface::setCompany(CompanyNode* root) {
	if (companyRoot != root) {
		delete companyRoot;
		companyRoot = root;
	}
}

Memento* ApplicationInterface::createMemento(MementoType type) {
	switch (type) {
	case MementoType::company:
		if (companyRoot == nullptr) return nullptr;
		return new Memento(companyRoot->clone(), MementoType::company);
	case MementoType::workflow:
		if (workflowRoot == nullptr) return nullptr;
		return new Memento(workflowRoot->clone(), MementoType::workflow);
	default:
		return nullptr;
	}
}

void ApplicationInterface::setMemento(Memento* memento) {
	switch (memento->getType()) {
	case MementoType::company:
		delete companyRoot;
		companyRoot = ((CompanyNode*)memento->getRoot())->clone();
		break;
	case MementoType::workflow:
		delete workflowRoot;
		workflowRoot = ((WorkNode*)memento->getRoot())->clone();

		break;
	default:
		break;
	}
}

ApplicationInterface::~ApplicationInterface() {
	if (workflowRoot) {
		delete workflowRoot;

	}
	if (companyRoot) {

		delete companyRoot;
	}
}
