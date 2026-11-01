#include "catch.hpp"
#include "FileSystem.h"

#include "WorkNode.h"
#include "Project.h"
#include "ApplicationInterface.h"
#include "Team.h"
#include "WorkUnit.h"
#include "EmployeeProfession.h"
#include "ConcreteEmployeeBuilder.h"
#include "EmployeeDirector.h"
#include "Memento.h"
#include "ConcreteWorkMediator.h"
#include "Employee.h"
#include "Manager.h"
#include "EmployeeCompetency.h"
#include "CompetencyLevel.h"
#include "Mediator.h"
#include <iostream>
#include <sstream>

void CompareWorkflowTrees(WorkNode* root1, WorkNode* root2) {
	int idx = 0;
	WorkNode* child1;
	WorkNode* child2;

	child1 = root1->getChild(idx);
	child2 = root2->getChild(idx);
	INFO("child1: " << child1 << " or child2: " << child2 << " is a null pointer");
	CHECK((child1 != nullptr) == (child2 != nullptr));

	while (child1 != nullptr && child2 != nullptr) {
		CHECK(child1->getMaxRetries() == child2->getMaxRetries());
		CHECK(child1->getName() == child2->getName());
		CHECK(child1->getStateName() == child2->getStateName());

		const auto& prereq1 = child1->getPrerequisites();
		const auto& prereq2 = child2->getPrerequisites();
		CHECK(prereq1.size() == prereq2.size());
		if (prereq1.size() == prereq2.size()) {

			for (int i = 0;i < prereq1.size();i++) {
				CHECK(prereq1[i] == prereq2[i]);
			}
		}
		CompareWorkflowTrees(child1, child2);

		idx++;
		child1 = child1->getChild(idx);
		child2 = child2->getChild(idx);

		INFO("child1: " << child1 << " or child2: " << child2 << " is a null pointer");
		CHECK((child1 != nullptr) == (child2 != nullptr));



	}
}

TEST_CASE("Filesystem works correctly", "[filesystem]") {
	ApplicationInterface face;
	FileSystem fs;

	WorkNode* root = new Project(5, "Test Project");

	WorkNode* part1 = new Project(5, "Part 1");

	WorkNode* work11 = new WorkUnit("Work11", "Artist");
	WorkNode* work12 = new WorkUnit("Work12", "Developer");

	part1->add(work11);
	part1->add(work12);

	WorkNode* part2 = new Project(5, "Part 2");
	WorkNode* work21 = new WorkUnit("Work21", "Designer");
	WorkNode* work22 = new WorkUnit("Work22", "Accountant");

	part2->add(work21);
	part2->add(work22);

	root->add(part1);
	root->add(part2);

	face.setWorkflow(root);

	SECTION("Saving and loading a workflow to and from filesystem reproduces the corrent tree structure") {

		Memento* workflowMem = face.createMemento(MementoType::workflow);


		fs.saveMemento("Test Workflow", workflowMem);

		Memento* workflowMem2 = fs.loadMemento("Test Workflow");
		CompareWorkflowTrees((WorkNode*)(workflowMem->getRoot()), (WorkNode*)(workflowMem2->getRoot()));
	}


}


TEST_CASE("Chain: intern starts a unit and senior finishes it", "[chain]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	Team team("Coders", "coder");
	team.addMember(director.makeIntern("Ian", "coder"));
	WorkUnit unit("login screen", "coder");

	team.assignWork(&unit);
	CHECK(unit.isOngoing());

	team.addMember(director.makeSenior("Sam", "coder"));
	team.assignWork(&unit);
	CHECK(unit.isCompleted());
}	

class SpyWorkMediator : public WorkMediator {
public:
	std::vector<std::pair<WorkNode*, std::string>> calls;
	void notifyColleagues(WorkNode* node, std::string msg) {
		calls.push_back({ node, msg });
	}
};

void completeUnit(WorkNode& node) {
	for (int i = 0; i < 4 && !node.isCompleted(); i++) {
		node.advanceState();
	}
}

TEST_CASE("Mediator: notifies every colleague except the sender", "[mediator]") {
	ConcreteWorkMediator mediator;
	WorkUnit sender("A", "coder");
	WorkUnit b("B", "coder");
	WorkUnit c("C", "coder");
	sender.addPrerequisite("A");
	b.addPrerequisite("A");
	c.addPrerequisite("Z");
	mediator.colleagues = { &sender, &b, &c };

	mediator.notifyColleagues(&sender, "A");

	CHECK(sender.getPrerequisites().size() == 1);
	CHECK(b.getPrerequisites().empty());
	CHECK(c.getPrerequisites().size() == 1);
}

TEST_CASE("Mediator: a waiting node becomes ready once all prerequisites are cleared", "[mediator]") {
	ConcreteWorkMediator mediator;
	WorkUnit a("A", "coder");
	WorkUnit b("B", "coder");
	WorkUnit c("C", "coder");
	c.addPrerequisite("A");
	c.addPrerequisite("B");
	mediator.colleagues = { &a, &b, &c };

	mediator.notifyColleagues(&a, "A");
	CHECK(c.getStateName() == "WaitingForPrerequisites");

	mediator.notifyColleagues(&b, "B");
	CHECK(c.getPrerequisites().empty());
	CHECK(c.isReady());
}

TEST_CASE("Mediator: a message matching no prerequisite changes nothing", "[mediator]") {
	ConcreteWorkMediator mediator;
	WorkUnit a("A", "coder");
	WorkUnit b("B", "coder");
	b.addPrerequisite("A");
	mediator.colleagues = { &a, &b };

	mediator.notifyColleagues(&a, "Unrelated");

	CHECK(b.getPrerequisites().size() == 1);
	CHECK(b.getStateName() == "WaitingForPrerequisites");
}

TEST_CASE("Mediator: a node only notifies when it becomes completed", "[mediator]") {
	SpyWorkMediator spy;
	WorkUnit a("A", "coder");
	a.setMediator(&spy);

	SECTION("earlier transitions do not notify") {
		a.advanceState();
		a.advanceState();
		CHECK(a.isEscalated());
		CHECK(spy.calls.empty());
	}

	SECTION("completing notifies once with the node's name") {
		completeUnit(a);
		REQUIRE(spy.calls.size() == 1);
		CHECK(spy.calls[0].first == &a);
		CHECK(spy.calls[0].second == "A");
	}

	SECTION("a node with no mediator completes without error") {
		WorkUnit lone("Lone", "coder");
		CHECK_NOTHROW(completeUnit(lone));
		CHECK(lone.isCompleted());
	}
}

TEST_CASE("Mediator: completing a node unblocks its dependent", "[mediator]") {
	ConcreteWorkMediator mediator;
	WorkUnit design("Design", "designer");
	WorkUnit build("Build", "coder");
	build.addPrerequisite("Design");
	mediator.colleagues = { &design, &build };
	design.setMediator(&mediator);
	build.setMediator(&mediator);

	completeUnit(design);

	CHECK(build.getPrerequisites().empty());
	CHECK(build.isReady());
}

TEST_CASE("Mediator: completions cascade through a dependency chain", "[mediator]") {
	ConcreteWorkMediator mediator;
	WorkUnit a("A", "coder");
	WorkUnit b("B", "coder");
	WorkUnit c("C", "coder");
	b.addPrerequisite("A");
	c.addPrerequisite("B");
	mediator.colleagues = { &a, &b, &c };
	a.setMediator(&mediator);
	b.setMediator(&mediator);
	c.setMediator(&mediator);

	completeUnit(a);
	CHECK(b.isReady());
	CHECK(c.getStateName() == "WaitingForPrerequisites");

	completeUnit(b);
	CHECK(c.isReady());
}

TEST_CASE("WorkNode: a new node starts ready with no parent or prerequisites", "[worknode]") {
	WorkUnit unit("Unit", "coder");

	CHECK(unit.getName() == "Unit");
	CHECK(unit.getStateName() == "Ready");
	CHECK(unit.getParent() == nullptr);
	CHECK(unit.getPrerequisites().empty());
	CHECK_FALSE(unit.isWaitingForPrerequisites());
	CHECK(unit.getMaxRetries() == 3);
	CHECK(unit.hasRetriesLeft());
}

TEST_CASE("WorkNode: setState by name selects the matching state", "[worknode]") {
	WorkUnit unit("Unit", "coder");
	const char* names[] = { "Ready", "Ongoing", "Escalated", "Completed", "Cancelled", "Failed", "WaitingForPrerequisites" };

	for (int i = 0; i < 7; i++) {
		unit.setState(names[i]);
		CHECK(unit.getStateName() == names[i]);
	}

	SECTION("an unknown name leaves the state unchanged") {
		unit.setState("Ongoing");
		unit.setState("NotAState");
		CHECK(unit.isOngoing());
	}

	SECTION("a null state leaves the state unchanged") {
		unit.setState("Ready");
		unit.setState((WorkState*)nullptr);
		CHECK(unit.isReady());
	}
}

TEST_CASE("States: a unit moves through every state to Completed", "[state]") {
	WorkUnit unit("Unit", "coder");

	CHECK(unit.isReady());
	unit.advanceState();
	CHECK(unit.isOngoing());
	unit.advanceState();
	CHECK(unit.getStateName() == "Escalated");
	unit.advanceState();
	CHECK(unit.isCompleted());
}

TEST_CASE("States: unmet prerequisites hold a node in WaitingForPrerequisites", "[state]") {
	WorkUnit unit("Unit", "coder");
	unit.addPrerequisite("Other");

	CHECK(unit.isWaitingForPrerequisites());
	unit.advanceState();
	CHECK(unit.getStateName() == "WaitingForPrerequisites");

	unit.update("Other");
	CHECK_FALSE(unit.isWaitingForPrerequisites());
	CHECK(unit.isReady());
}

TEST_CASE("States: Completed, Cancelled and Failed do not change when advanced", "[state]") {
	WorkUnit unit("Unit", "coder");
	const char* names[] = { "Completed", "Cancelled", "Failed" };

	for (int i = 0; i < 3; i++) {
		unit.setState(names[i]);
		unit.advanceState();
		CHECK(unit.getStateName() == names[i]);
	}
}

TEST_CASE("States: a project reports completion only when all of its children have", "[state]") {
	Project* project = new Project(3, "Project");
	WorkNode* first = new WorkUnit("First", "coder");
	WorkNode* second = new WorkUnit("Second", "coder");
	project->add(first);
	project->add(second);

	SECTION("allChildrenCompleted is true once the last child completes") {
		CHECK_FALSE(project->allChildrenCompleted());

		completeUnit(*first);
		CHECK_FALSE(project->allChildrenCompleted());

		completeUnit(*second);
		CHECK(project->allChildrenCompleted());
	}

	SECTION("a cancelled project is not completed by its children") {
		project->setState("Cancelled");

		completeUnit(*first);
		completeUnit(*second);
		CHECK(project->getStateName() == "Cancelled");
	}

	delete project;
}

TEST_CASE("WorkNode: reset sends a node back to Ready and uses up a retry", "[worknode]") {
	WorkUnit unit("Unit", "coder");

	SECTION("only Ongoing or Escalated nodes are reset") {
		unit.reset();
		CHECK(unit.isReady());
		CHECK(unit.hasRetriesLeft());

		unit.setState("Failed");
		unit.reset();
		CHECK(unit.getStateName() == "Failed");
		CHECK(unit.hasRetriesLeft());
	}

	SECTION("each reset uses up a retry until none are left") {
		for (int i = 0; i < unit.getMaxRetries(); i++) {
			unit.setState(i % 2 == 0 ? "Ongoing" : "Escalated");
			CHECK(unit.hasRetriesLeft());
			unit.reset();
			CHECK(unit.isReady());
		}

		CHECK_FALSE(unit.hasRetriesLeft());
	}
}

class SpyEmployeeMediator : public Mediator {
public:
	std::vector<std::pair<EmployeeComponent*, std::string>> calls;
	void notifyColleagues(EmployeeComponent* employeeComponent, std::string message) {
		calls.push_back({ employeeComponent, message });
	}
};

class RecordingEmployee : public Employee {
public:
	std::vector<std::string> received;
	RecordingEmployee(std::string name) : Employee(name) {}
	void receive(std::string message) {
		received.push_back(message);
	}
};

class QuietOutput {
	std::ostringstream sink;
	std::streambuf* old;
public:
	QuietOutput() : old(std::cout.rdbuf(sink.rdbuf())) {}
	~QuietOutput() { std::cout.rdbuf(old); }
};

void quietDoWork(EmployeeComponent& employee, WorkUnit* unit) {
	QuietOutput quiet;
	employee.doWork(unit);
}

bool quietHandleWork(EmployeeComponent& employee, WorkUnit* unit) {
	QuietOutput quiet;
	return employee.handleWork(unit);
}

void quietAssign(Team& team, WorkUnit* unit) {
	QuietOutput quiet;
	team.assignWork(unit);
}

TEST_CASE("Employee: decorators report the wrapped employee's details", "[employee]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	EmployeeComponent* intern = director.makeIntern("Ian", "coder");
	EmployeeComponent* senior = director.makeSenior("Sam", "coder");
	Employee plain("Pat");

	CHECK(senior->getName() == "Sam");
	CHECK(senior->getKind() == "Profession");
	CHECK(senior->compareProfession("coder"));
	CHECK_FALSE(senior->compareProfession("artist"));
	CHECK(senior->compareCompetency(CompetencyLevel::SENIOR));
	CHECK(intern->compareCompetency(CompetencyLevel::INTERN));
	CHECK_FALSE(intern->compareCompetency(CompetencyLevel::JUNIOR));
	CHECK_FALSE(plain.compareCompetency(CompetencyLevel::INTERN));

	delete intern;
	delete senior;
}

TEST_CASE("Employee: doWork tells the mediator who worked on what", "[employee]") {
	Employee worker("Ann");
	SpyEmployeeMediator spy;
	WorkUnit unit("Report", "coder");

	SECTION("the mediator is notified") {
		worker.setMediator(&spy);
		quietDoWork(worker, &unit);
		REQUIRE(spy.calls.size() == 1);
		CHECK(spy.calls[0].first == &worker);
		CHECK(spy.calls[0].second == "Ann worked on Report");
	}

	SECTION("no mediator is fine") {
		CHECK_NOTHROW(quietDoWork(worker, &unit));
	}

	SECTION("null work does nothing") {
		worker.setMediator(&spy);
		quietDoWork(worker, nullptr);
		CHECK(spy.calls.empty());
	}
}

TEST_CASE("Employee: handleWork only advances work the competency allows", "[employee]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	EmployeeComponent* intern = director.makeIntern("Ian", "coder");
	EmployeeComponent* senior = director.makeSenior("Sam", "coder");
	WorkUnit unit("Task", "coder");

	SECTION("an intern can start ready work") {
		CHECK(quietHandleWork(*intern, &unit));
		CHECK(unit.isOngoing());
	}

	SECTION("an intern cannot take ongoing work further") {
		unit.advanceState();
		CHECK_FALSE(quietHandleWork(*intern, &unit));
		CHECK(unit.isOngoing());
	}

	SECTION("a senior can complete escalated work") {
		unit.setState("Escalated");
		CHECK(quietHandleWork(*senior, &unit));
		CHECK(unit.isCompleted());
	}

	SECTION("work passes along to the successor") {
		unit.advanceState();
		intern->setSuccessor(senior);
		CHECK(quietHandleWork(*intern, &unit));
		CHECK(unit.getStateName() == "Escalated");
	}

	SECTION("null work is refused") {
		CHECK_FALSE(intern->handleWork(nullptr));
	}

	delete intern;
	delete senior;
}

TEST_CASE("Employee: clone copies the whole decorated employee", "[employee]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	EmployeeComponent* senior = director.makeSenior("Sam", "coder");
	EmployeeComponent* copy = senior->clone();
	Manager boss("Bea");
	EmployeeComponent* bossCopy = boss.clone();

	CHECK(copy != senior);
	CHECK(copy->getName() == "Sam");
	CHECK(copy->compareProfession("coder"));
	CHECK(copy->compareCompetency(CompetencyLevel::SENIOR));
	CHECK(bossCopy->getKind() == "Manager");
	CHECK(bossCopy->getName() == "Bea");

	delete senior;
	delete copy;
	delete bossCopy;
}

TEST_CASE("Team: members are notified when a colleague does work", "[team]") {
	Team team("Team", "");
	RecordingEmployee* a = new RecordingEmployee("A");
	RecordingEmployee* b = new RecordingEmployee("B");
	RecordingEmployee* c = new RecordingEmployee("C");
	team.addMember(a);
	team.addMember(b);
	team.addMember(c);
	WorkUnit unit("Task");

	quietDoWork(*a, &unit);

	CHECK(a->received.empty());
	REQUIRE(b->received.size() == 1);
	CHECK(b->received[0] == "A worked on Task");
	CHECK(c->received.size() == 1);
}

TEST_CASE("Team: addMember checks the member's profession", "[team]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	Team team("Coders", "coder");

	CHECK(team.getProfession() == "coder");

	SECTION("null is rejected") {
		CHECK_FALSE(team.addMember(nullptr));
	}

	SECTION("a member of another profession is rejected") {
		EmployeeComponent* artist = director.makeIntern("Ann", "artist");
		CHECK_FALSE(team.addMember(artist));
		delete artist;
	}

	SECTION("an employee with no profession is rejected") {
		Employee* plain = new Employee("Pat");
		CHECK_FALSE(team.addMember(plain));
		delete plain;
	}

	SECTION("a matching member is accepted") {
		CHECK(team.addMember(director.makeIntern("Ian", "coder")));
	}

	SECTION("a team with no profession accepts anyone") {
		Team open("Open", "");
		CHECK(open.addMember(new Employee("Pat")));
	}
}

TEST_CASE("Team: assignWork ignores work it cannot use", "[team]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	Team team("Coders", "coder");
	team.addMember(director.makeSenior("Sam", "coder"));
	EmployeeProfession::registerProfession("designer");

	SECTION("work for another profession") {
		WorkUnit unit("Logo", "designer");
		quietAssign(team, &unit);
		CHECK(unit.isReady());
	}

	SECTION("work needing a profession nobody registered") {
		WorkUnit unit("Moon", "astronaut");
		quietAssign(team, &unit);
		CHECK(unit.isReady());
	}

	SECTION("a team with no members") {
		Team empty("Empty", "");
		WorkUnit unit("Task", "coder");
		quietAssign(empty, &unit);
		CHECK(unit.isReady());
	}

	SECTION("null work") {
		CHECK_NOTHROW(quietAssign(team, nullptr));
	}
}

TEST_CASE("Team: assignWork completes work or fails it after retries", "[team]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	Team team("Team", "");

	SECTION("a senior takes work from Ready to Completed") {
		team.addMember(director.makeSenior("Sam", "coder"));
		WorkUnit unit("Task", "coder");

		quietAssign(team, &unit);
		CHECK(unit.isCompleted());
	}

	SECTION("work nobody can do is retried and then failed") {
		team.addMember(new Employee("Eve"));
		WorkUnit unit("Task");

		for (int i = 0; i < unit.getMaxRetries(); i++) {
			unit.setState("Ongoing");
			quietAssign(team, &unit);
			CHECK(unit.isReady());
		}
		unit.setState("Ongoing");
		quietAssign(team, &unit);
		CHECK(unit.getStateName() == "Failed");
	}
}

TEST_CASE("Team: clone copies the profession and members", "[team]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	Team original("Coders", "coder");
	original.addMember(director.makeSenior("Sam", "coder"));

	Team* copy = dynamic_cast<Team*>(original.clone());
	REQUIRE(copy != nullptr);
	CHECK(copy != &original);
	CHECK(copy->getProfession() == "coder");

	WorkUnit unit("Task", "coder");
	quietAssign(*copy, &unit);
	CHECK(unit.isCompleted());

	delete copy;
}

std::string capturedDoWork(EmployeeComponent& employee, WorkUnit* unit) {
	std::ostringstream sink;
	std::streambuf* old = std::cout.rdbuf(sink.rdbuf());
	employee.doWork(unit);
	std::cout.rdbuf(old);
	return sink.str();
}

TEST_CASE("Employee: getCore returns the innermost employee", "[employee]") {
	Employee plain("Pat");
	EmployeeProfession* profession = new EmployeeProfession("coder");
	EmployeeCompetency* competency = new EmployeeCompetency(CompetencyLevel::SENIOR);
	Employee* core = new Employee("Sam");
	profession->wrap(competency);
	profession->wrap(core);

	CHECK(plain.getCore() == &plain);
	CHECK(core->getCore() == core);
	CHECK(competency->getCore() == core);
	CHECK(profession->getCore() == core);

	delete profession;
}

TEST_CASE("Team: a decorated member does not receive its own message", "[team]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	Team team("Team", "");
	EmployeeComponent* sam = director.makeSenior("Sam", "coder");
	EmployeeComponent* ann = director.makeSenior("Ann", "coder");
	team.addMember(sam);
	team.addMember(ann);
	WorkUnit unit("Task", "coder");

	std::string output = capturedDoWork(*sam, &unit);

	CHECK(output.find("Sam received") == std::string::npos);
	CHECK(output.find("Ann received: Sam worked on Task") != std::string::npos);
}

#include <cstdio>
#include <cstdlib>
#include "Department.h"
#include "Project.h"

static std::string tempSavePath(const std::string& name){
	const char* folder = std::getenv("TEMP");
	if(folder == nullptr){
		folder = std::getenv("TMPDIR");
	}
	if(folder == nullptr){
		folder = "/tmp";
	}
	return std::string(folder) + "/" + name;
}

TEST_CASE("Memento: remembers the type it was created with", "[memento]") {
	ApplicationInterface face;
	face.getCompany();
	face.getWorkflow();

	Memento* company = face.createMemento(MementoType::company);
	Memento* workflow = face.createMemento(MementoType::workflow);
	REQUIRE(company != nullptr);
	REQUIRE(workflow != nullptr);
	CHECK(company->getType() == MementoType::company);
	CHECK(workflow->getType() == MementoType::workflow);
	delete company;
	delete workflow;
}

TEST_CASE("Memento: nothing to snapshot gives no memento", "[memento]") {
	ApplicationInterface face;
	CHECK(face.createMemento(MementoType::company) == nullptr);
	CHECK(face.createMemento(MementoType::workflow) == nullptr);
}

TEST_CASE("Save/load: a workflow keeps names, states and prerequisites", "[saveload]") {
	ApplicationInterface face;
	Project* phase = face.createProject(nullptr, "Phase 1");
	REQUIRE(phase != nullptr);
	WorkUnit* design = face.addSubcomponent(phase, "Design", "Artist");
	WorkUnit* build = face.addSubcomponent(phase, "Build", "Developer");
	REQUIRE(design != nullptr);
	REQUIRE(build != nullptr);
	build->addPrerequisite("Design");
	design->setState("Ongoing");

	std::string path = tempSavePath("tividy_test_workflow.txt");
	REQUIRE(face.saveToFile(MementoType::workflow, path));

	ApplicationInterface other;
	REQUIRE(other.loadFromFile(MementoType::workflow, path));
	CompareWorkflowTrees(face.getWorkflow(), other.getWorkflow());

	WorkNode* phaseCopy = other.getWorkflow()->getChild(0);
	REQUIRE(phaseCopy != nullptr);
	CHECK(phaseCopy->getName() == "Phase 1");
	WorkUnit* buildCopy = dynamic_cast<WorkUnit*>(phaseCopy->getChild(1));
	REQUIRE(buildCopy != nullptr);
	CHECK(buildCopy->getRequiredProfession() == "Developer");
	CHECK(buildCopy->getPrerequisites().size() == 1);
	CHECK(phaseCopy->getChild(0)->getStateName() == "Ongoing");
	std::remove(path.c_str());
}

TEST_CASE("Save/load: a loaded workflow still drives the mediator", "[saveload]") {
	ApplicationInterface face;
	WorkUnit* a = face.addSubcomponent(nullptr, "A", "");
	WorkUnit* b = face.addSubcomponent(nullptr, "B", "");
	b->addPrerequisite("A");

	std::string path = tempSavePath("tividy_test_mediator.txt");
	REQUIRE(face.saveToFile(MementoType::workflow, path));
	ApplicationInterface other;
	REQUIRE(other.loadFromFile(MementoType::workflow, path));

	auto units = other.getWorkUnits();
	REQUIRE(units.size() == 2);
	completeUnit(*units[0]);
	CHECK(units[1]->getPrerequisites().empty());
	CHECK(units[1]->isReady());
	(void)a;
	std::remove(path.c_str());
}

TEST_CASE("Save/load: a company keeps departments, teams and decorated employees", "[saveload]") {
	ApplicationInterface face;
	Department* dept = face.addDepartment(nullptr, "Engineering");
	Department* sub = face.addDepartment(dept, "Platform");
	Team* team = face.addTeam(sub, "Core Devs", "Developer");
	Team* empty = face.addTeam(dept, "Nobody Yet", "Designer");
	REQUIRE(team != nullptr);
	REQUIRE(face.addEmployee(team, "Ann", "Developer", "Senior", false));
	REQUIRE(face.addEmployee(team, "Bob", "Developer", "Intern", true));
	REQUIRE(face.setCompetency(team, 1, "Junior"));
	CHECK_FALSE(face.addEmployee(team, "Wrong", "Designer", "Intern", false));
	(void)empty;

	std::string path = tempSavePath("tividy_test_company.txt");
	REQUIRE(face.saveToFile(MementoType::company, path));

	ApplicationInterface other;
	REQUIRE(other.loadFromFile(MementoType::company, path));

	REQUIRE(other.getCompany()->getChild(0) != nullptr);
	CompanyNode* deptCopy = other.getCompany()->getChild(0);
	CHECK(deptCopy->getName() == "Engineering");
	CompanyNode* subCopy = deptCopy->getChild(0);
	REQUIRE(subCopy != nullptr);
	CHECK(subCopy->getName() == "Platform");
	Team* teamCopy = dynamic_cast<Team*>(subCopy->getChild(0));
	REQUIRE(teamCopy != nullptr);
	CHECK(teamCopy->getName() == "Core Devs");
	CHECK(teamCopy->getProfession() == "Developer");
	REQUIRE(teamCopy->getMemberCount() == 2);

	EmployeeComponent* ann = teamCopy->getMember(0);
	CHECK(ann->getName() == "Ann");
	CHECK(ann->getKind() == "Profession");
	CHECK(ann->compareCompetency(CompetencyLevel::SENIOR));
	CHECK(ann->getCore()->getKind() == "Employee");

	EmployeeComponent* bob = teamCopy->getMember(1);
	CHECK(bob->getName() == "Bob");
	CHECK(bob->getCore()->getKind() == "Manager");
	CHECK(bob->compareCompetency(CompetencyLevel::JUNIOR));

	CompanyNode* emptyCopy = deptCopy->getChild(1);
	REQUIRE(dynamic_cast<Team*>(emptyCopy) != nullptr);
	CHECK(dynamic_cast<Team*>(emptyCopy)->getMemberCount() == 0);
	std::remove(path.c_str());
}

TEST_CASE("Save/load: wrong or missing files are rejected and change nothing", "[saveload]") {
	ApplicationInterface face;
	face.createProject(nullptr, "Keep me");
	face.addDepartment(nullptr, "Dept");

	std::string path = tempSavePath("tividy_test_wrongtype.txt");
	REQUIRE(face.saveToFile(MementoType::company, path));

	CHECK_FALSE(face.loadFromFile(MementoType::workflow, path));
	CHECK(face.getWorkflow()->getChild(0)->getName() == "Keep me");
	CHECK_FALSE(face.loadFromFile(MementoType::company, tempSavePath("does_not_exist_tividy.txt")));
	std::remove(path.c_str());
}

TEST_CASE("Save/load: work can be assigned to a team after loading", "[saveload]") {
	ApplicationInterface face;
	Team* team = face.addTeam(nullptr, "Devs", "Developer");
	face.addEmployee(team, "Sam", "Developer", "Senior", false);
	WorkUnit* unit = face.addSubcomponent(nullptr, "Ship it", "Developer");
	REQUIRE(unit != nullptr);

	std::string path = tempSavePath("tividy_test_assign.txt");
	REQUIRE(face.saveToFile(MementoType::workflow, path));
	REQUIRE(face.loadFromFile(MementoType::workflow, path));

	WorkUnit* loaded = face.getWorkUnits()[0];
	std::string result = face.assignWork(loaded, face.getTeams("Developer")[0]);
	CHECK(loaded->isCompleted());
	CHECK(result.find("Completed") != std::string::npos);
	std::remove(path.c_str());
}


TEST_CASE("Workflow: prerequisites can be added and removed safely", "[workflow]") {
	ApplicationInterface face;
	Project* phase = face.createProject(nullptr, "Phase");
	WorkUnit* a = face.addSubcomponent(phase, "A", "");
	WorkUnit* b = face.addSubcomponent(phase, "B", "");
	WorkUnit* c = face.addSubcomponent(nullptr, "C", "");

	CHECK(face.addPrerequisite(b, "A"));
	CHECK_FALSE(face.addPrerequisite(b, "A"));
	CHECK_FALSE(face.addPrerequisite(b, "B"));
	CHECK_FALSE(face.addPrerequisite(b, "Nope"));
	CHECK_FALSE(face.addPrerequisite(b, "Phase"));
	CHECK_FALSE(face.addPrerequisite(a, "B"));
	CHECK(face.addPrerequisite(c, "B"));
	CHECK_FALSE(face.addPrerequisite(a, "C"));

	CHECK(face.removePrerequisite(b, "A"));
	CHECK(b->isReady());
	CHECK_FALSE(face.removePrerequisite(b, "A"));

	completeUnit(*a);
	CHECK_FALSE(face.addPrerequisite(c, "A"));
}

TEST_CASE("Workflow: removing a node removes prerequisites on it and keeps names unique", "[workflow]") {
	ApplicationInterface face;
	WorkUnit* a = face.addSubcomponent(nullptr, "A", "");
	WorkUnit* b = face.addSubcomponent(nullptr, "B", "");
	CHECK(face.addSubcomponent(nullptr, "A", "") == nullptr);
	REQUIRE(face.addPrerequisite(b, "A"));

	REQUIRE(face.removeWorkNode(a));
	CHECK(b->getPrerequisites().empty());
	CHECK(face.getWorkUnits().size() == 1);
	CHECK_FALSE(face.removeWorkNode(face.getWorkflow()));
}

TEST_CASE("Company: members can be removed and the chain still works", "[company]") {
	ApplicationInterface face;
	Team* team = face.addTeam(nullptr, "Devs", "Developer");
	REQUIRE(face.addEmployee(team, "Intern", "Developer", "Intern", false));
	REQUIRE(face.addEmployee(team, "Mid", "Developer", "Junior", false));
	REQUIRE(face.addEmployee(team, "Boss", "Developer", "Senior", true));
	REQUIRE(team->removeMember(1));
	CHECK(team->getMemberCount() == 2);

	WorkUnit* unit = face.addSubcomponent(nullptr, "Task", "Developer");
	face.assignWork(unit, team);
	CHECK(unit->isCompleted());

	face.getCompany()->remove(team);
	delete team;
	CHECK(face.getTeams().empty());
}

TEST_CASE("WorkIterator: can walk every node, not just ready units", "[iterator]"){
	ApplicationInterface face;
	Project* phase = face.createProject(nullptr, "Phase");
	WorkUnit* a = face.addSubcomponent(phase, "A", "");
	WorkUnit* b = face.addSubcomponent(phase, "B", "");
	REQUIRE(face.addPrerequisite(b, "A"));

	int all = 0;
	WorkIterator everything(face.getWorkflow(), false);
	for(everything.first(); !everything.isDone(); everything.next()){
		all++;
	}
	CHECK(all == 4);

	int ready = 0;
	WorkIterator readyOnly(face.getWorkflow());
	for(readyOnly.first(); !readyOnly.isDone(); readyOnly.next()){
		ready++;
	}
	CHECK(ready == 1);
	CHECK(a->isReady());
}

TEST_CASE("Company: setCompetency replaces the level and keeps name, role and profession", "[company]"){
	ApplicationInterface face;
	Team* team = face.addTeam(nullptr, "Devs", "Developer");
	REQUIRE(face.addEmployee(team, "Boss", "Developer", "Senior", true));
	REQUIRE(face.setCompetency(team, 0, "Intern"));
	EmployeeComponent* boss = team->getMember(0);
	CHECK(team->getMemberCount() == 1);
	CHECK(boss->getName() == "Boss");
	CHECK(boss->getCore()->getKind() == "Manager");
	CHECK(boss->compareProfession("Developer"));
	CHECK(boss->compareCompetency(CompetencyLevel::INTERN));
	CHECK_FALSE(boss->compareCompetency(CompetencyLevel::JUNIOR));
	CHECK_FALSE(face.setCompetency(team, 5, "Senior"));
	CHECK_FALSE(face.setCompetency(team, 0, "wizard"));
}


bool quietStep(Team& team, WorkUnit* unit) {
	QuietOutput quiet;
	return team.stepWork(unit);
}

TEST_CASE("Team: stepWork moves work on one state at a time", "[team][step]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	Team team("Team", "");
	team.addMember(director.makeSenior("Sam", "coder"));
	WorkUnit unit("Task", "coder");

	CHECK(quietStep(team, &unit));
	CHECK(unit.isOngoing());
	CHECK(quietStep(team, &unit));
	CHECK(unit.getStateName() == "Escalated");
	CHECK(quietStep(team, &unit));
	CHECK(unit.isCompleted());
	CHECK_FALSE(quietStep(team, &unit));
}

TEST_CASE("Team: stepWork stays put when nobody can take the next step", "[team][step]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	Team team("Team", "");
	team.addMember(director.makeIntern("Ian", "coder"));
	WorkUnit unit("Task", "coder");

	CHECK(quietStep(team, &unit));
	CHECK(unit.isOngoing());
	CHECK_FALSE(quietStep(team, &unit));
	CHECK(unit.isOngoing());
	CHECK_FALSE(quietStep(team, nullptr));
}

TEST_CASE("Workflow: assigned work can be stepped and cancelled", "[workflow][step]") {
	ApplicationInterface face;
	Team* team = face.addTeam(nullptr, "Devs", "Developer");
	REQUIRE(face.addEmployee(team, "Sam", "Developer", "Senior", false));
	WorkUnit* unit = face.addSubcomponent(nullptr, "Task", "Developer");
	WorkUnit* other = face.addSubcomponent(nullptr, "Other", "Developer");
	REQUIRE(unit != nullptr);
	REQUIRE(other != nullptr);

	SECTION("stepping needs an assignment") {
		REQUIRE(face.addPrerequisite(unit, "Other"));
		CHECK(face.stepWork(unit).find("still waiting") != std::string::npos);
		REQUIRE(face.removePrerequisite(unit, "Other"));
		CHECK(face.stepWork(unit).find("not assigned") != std::string::npos);
		CHECK(unit->isReady());
	}

	SECTION("assigning alone does no work, each step moves one state") {
		face.assignOnly(unit, team);
		CHECK(unit->isReady());
		QuietOutput quiet;
		face.stepWork(unit);
		CHECK(unit->isOngoing());
		face.stepWork(unit);
		face.stepWork(unit);
		CHECK(unit->isCompleted());
		CHECK(face.stepWork(unit).find("already Completed") != std::string::npos);
	}

	SECTION("cancelling stops the work") {
		face.assignOnly(other, team);
		CHECK(face.cancelWork(other).find("cancelled") != std::string::npos);
		CHECK(other->getStateName() == "Cancelled");
		CHECK(face.stepWork(other).find("already Cancelled") != std::string::npos);
		CHECK(face.cancelWork(other).find("already Cancelled") != std::string::npos);
		CHECK_FALSE(face.cancelWork(nullptr).empty());
	}

	SECTION("work nobody can finish is retried and then failed") {
		Team* interns = face.addTeam(nullptr, "Interns", "Developer");
		REQUIRE(face.addEmployee(interns, "Ian", "Developer", "Intern", false));
		face.assignOnly(unit, interns);
		QuietOutput quiet;
		for (int attempt = 0; attempt < unit->getMaxRetries(); attempt++) {
			face.stepWork(unit);
			CHECK(unit->isOngoing());
			CHECK(face.stepWork(unit).find("another try") != std::string::npos);
			CHECK(unit->isReady());
		}
		face.stepWork(unit);
		CHECK(unit->isOngoing());
		CHECK(face.stepWork(unit).find("failed") != std::string::npos);
		CHECK(unit->getStateName() == "Failed");
		CHECK(face.stepWork(unit).find("already Failed") != std::string::npos);
	}

	SECTION("work nobody can finish can be cancelled instead") {
		Team* interns = face.addTeam(nullptr, "Interns", "Developer");
		REQUIRE(face.addEmployee(interns, "Ian", "Developer", "Intern", false));
		face.assignOnly(unit, interns);
		QuietOutput quiet;
		face.stepWork(unit);
		face.stepWork(unit);
		face.cancelWork(unit);
		CHECK(unit->getStateName() == "Cancelled");
	}

	SECTION("a deleted team cannot be stepped") {
		face.assignOnly(unit, team);
		face.getCompany()->remove(team);
		delete team;
		CHECK(face.stepWork(unit).find("still exists") != std::string::npos);
	}
}

TEST_CASE("Team: retryOrFail sends work back to Ready until retries run out", "[team][step]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	Team team("Team", "");
	team.addMember(director.makeIntern("Ian", "coder"));
	WorkUnit unit("Task", "coder");
	REQUIRE(quietStep(team, &unit));
	REQUIRE(unit.isOngoing());

	for (int i = 0; i < unit.getMaxRetries(); i++) {
		CHECK(team.retryOrFail(&unit));
		CHECK(unit.isReady());
		quietStep(team, &unit);
	}
	CHECK(team.retryOrFail(&unit));
	CHECK(unit.getStateName() == "Failed");
	CHECK_FALSE(team.retryOrFail(&unit));
	CHECK_FALSE(team.retryOrFail(nullptr));
}

TEST_CASE("Workflow: work can be forced to fail and forced into a state", "[workflow][step]") {
	ApplicationInterface face;
	Team* team = face.addTeam(nullptr, "Devs", "Developer");
	REQUIRE(face.addEmployee(team, "Sam", "Developer", "Senior", false));
	WorkUnit* unit = face.addSubcomponent(nullptr, "Task", "Developer");
	WorkUnit* next = face.addSubcomponent(nullptr, "Next", "Developer");
	REQUIRE(face.addPrerequisite(next, "Task"));

	SECTION("forcing a fail needs an assignment") {
		CHECK(face.failWork(unit).find("not assigned") != std::string::npos);
		CHECK(unit->isReady());
	}

	SECTION("a forced fail retries until the retries run out") {
		face.assignOnly(unit, team);
		QuietOutput quiet;
		for (int attempt = 0; attempt < unit->getMaxRetries(); attempt++) {
			face.stepWork(unit);
			CHECK(unit->isOngoing());
			CHECK(face.failWork(unit).find("another try") != std::string::npos);
			CHECK(unit->isReady());
		}
		face.stepWork(unit);
		CHECK(face.failWork(unit).find("no retries left") != std::string::npos);
		CHECK(unit->getStateName() == "Failed");
		CHECK(face.failWork(unit).find("already Failed") != std::string::npos);
	}

	SECTION("forcing a state sets it") {
		CHECK(face.forceState(unit, "Cancelled").find("Cancelled") != std::string::npos);
		CHECK(unit->getStateName() == "Cancelled");
		CHECK(face.forceState(unit, "Ongoing").find("Ongoing") != std::string::npos);
		CHECK(unit->isOngoing());
		CHECK(face.forceState(unit, "Ongoing").find("already") != std::string::npos);
	}

	SECTION("forcing Completed lets the work waiting on it start") {
		CHECK(next->getStateName() == "WaitingForPrerequisites");
		face.forceState(unit, "Completed");
		CHECK(unit->isCompleted());
		CHECK(next->isReady());
	}

	SECTION("bad input is refused") {
		CHECK(face.forceState(unit, "Sleeping").find("not a state") != std::string::npos);
		CHECK(unit->isReady());
		CHECK_FALSE(face.forceState(nullptr, "Ready").empty());
		CHECK_FALSE(face.failWork(nullptr).empty());
	}
}

TEST_CASE("Company: Children get assigned correctly","[company]")
{
	CompanyNode* root=new Department("root");
	CompanyNode* child0=new Department("child0");
	CompanyNode* child1=new Department("child1");
	CompanyNode* child2=new Department("child2");
	CompanyNode* team0=new Team("team0","coder");
	CompanyNode* team1=new Team("team1","coder");

	root->add(child0);
	root->add(child1);
	child0->add(child2);
	child2->add(team0);
	team0->add(team1);

	CHECK(root->getChild(0)==child0);
	CHECK(root->getChild(1)==child1);
	CHECK(root->getChild(2)==nullptr);
	CHECK(root->getChild(-11)==nullptr);
	CHECK(child2->getChild(0)==team0);
	CHECK(team0->getChild(0)==nullptr);
}

#include "EmployeeDetail.h"
#include "TeamIterator.h"
#include "Iterator.h"
#include "WorkIterator.h"
#include <vector>
#include <string>

TEST_CASE("Employee: a plain employee only has a name", "[employee]") {
	Employee alice("Alice");

	CHECK(alice.getName() == "Alice");
	CHECK(alice.getKind() == "Employee");
	CHECK_FALSE(alice.compareProfession("coder"));
}

TEST_CASE("Employee: wrapping adds a profession and a level and keeps the name", "[employee]") {
	EmployeeProfession::registerProfession("coder");
	EmployeeComponent* bob = new EmployeeProfession("coder");
	bob->wrap(new EmployeeCompetency(CompetencyLevel::JUNIOR));
	bob->wrap(new Employee("Bob"));

	CHECK(bob->getName() == "Bob");
	CHECK(bob->compareProfession("coder"));
	CHECK_FALSE(bob->compareProfession("artist"));
	CHECK(bob->compareCompetency(CompetencyLevel::INTERN));
	CHECK(bob->compareCompetency(CompetencyLevel::JUNIOR));
	CHECK_FALSE(bob->compareCompetency(CompetencyLevel::SENIOR));
	CHECK(bob->getCore()->getKind() == "Employee");

	delete bob;
}

TEST_CASE("Employee: one employee can hold two professions", "[employee]") {
	EmployeeProfession::registerProfession("coder");
	EmployeeProfession::registerProfession("artist");
	EmployeeComponent* carol = new EmployeeProfession("coder");
	carol->wrap(new EmployeeProfession("artist"));
	carol->wrap(new Employee("Carol"));

	CHECK(carol->getName() == "Carol");
	CHECK(carol->compareProfession("coder"));
	CHECK(carol->compareProfession("artist"));

	delete carol;
}

TEST_CASE("Employee: the director's junior has a junior level", "[employee]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	EmployeeComponent* junior = director.makeJunior("Jo", "coder");

	CHECK(junior->getName() == "Jo");
	CHECK(junior->getCompetency() == "Junior");
	CHECK(junior->compareCompetency(CompetencyLevel::JUNIOR));
	CHECK_FALSE(junior->compareCompetency(CompetencyLevel::SENIOR));

	delete junior;
}

TEST_CASE("Employee: a manager stays a manager inside the layers", "[employee]") {
	ConcreteEmployeeBuilder builder;
	builder.reset();
	builder.addProfession("Manager");
	builder.addCompetency("Senior");
	EmployeeComponent* mia = builder.finish("Mia", true);

	CHECK(mia->getName() == "Mia");
	CHECK(mia->getCore()->getKind() == "Manager");
	CHECK(mia->compareCompetency(CompetencyLevel::SENIOR));

	delete mia;
}

TEST_CASE("Employee: an unknown competency level is rejected", "[employee]") {
	ConcreteEmployeeBuilder builder;
	builder.reset();

	CHECK(EmployeeCompetency::getCompetencyLevel("Wizard") == CompetencyLevel::NONE);
	CHECK_FALSE(builder.addCompetency("Wizard"));

	builder.reset();
}

TEST_CASE("TeamIterator: finds teams by profession anywhere in the tree", "[iterator]") {
	Department company("Studio");
	Department* development = new Department("Development");
	Department* support = new Department("Support");
	development->add(new Team("Coders", "coder"));
	development->add(new Team("Artists", "artist"));
	support->add(new Team("Fixers", "coder"));
	company.add(development);
	company.add(support);

	std::vector<std::string> coderTeams;
	TeamIterator coders(&company, "coder");
	for (coders.first(); !coders.isDone(); coders.next()) {
		coderTeams.push_back(coders.currentItem()->getName());
	}
	REQUIRE(coderTeams.size() == 2);
	CHECK(coderTeams[0] == "Coders");
	CHECK(coderTeams[1] == "Fixers");

	int allTeams = 0;
	TeamIterator everyone(&company, "");
	for (everyone.first(); !everyone.isDone(); everyone.next()) {
		allTeams++;
	}
	CHECK(allTeams == 3);
}

std::vector<std::string> readyUnitNames(WorkNode* root) {
	std::vector<std::string> names;
	Iterator* it = root->createIterator();
	for (it->first(); !it->isDone(); it->next()) {
		names.push_back(it->currentItem()->getName());
	}
	delete it;
	return names;
}

TEST_CASE("WorkIterator: an empty project gives nothing", "[iterator]") {
	Project root(3, "Root");

	CHECK(readyUnitNames(&root).empty());
}

TEST_CASE("WorkIterator: only Ready work units come back, in order", "[iterator]") {
	Project root(3, "Root");
	WorkUnit* a = new WorkUnit("A", "coder");
	WorkUnit* b = new WorkUnit("B", "coder");
	WorkUnit* c = new WorkUnit("C", "coder");
	WorkUnit* d = new WorkUnit("D", "coder");
	root.add(a);
	root.add(b);
	root.add(c);
	root.add(d);
	b->setState("Completed");
	d->setState("WaitingForPrerequisites");

	std::vector<std::string> names = readyUnitNames(&root);
	REQUIRE(names.size() == 2);
	CHECK(names[0] == "A");
	CHECK(names[1] == "C");
}

TEST_CASE("WorkIterator: nested projects are walked depth first", "[iterator]") {
	Project root(3, "Root");
	Project* sub = new Project(3, "Sub");
	root.add(new WorkUnit("A", "coder"));
	root.add(sub);
	sub->add(new WorkUnit("B", "coder"));
	sub->add(new WorkUnit("C", "coder"));
	root.add(new WorkUnit("D", "coder"));

	std::vector<std::string> names = readyUnitNames(&root);
	REQUIRE(names.size() == 4);
	CHECK(names[0] == "A");
	CHECK(names[1] == "B");
	CHECK(names[2] == "C");
	CHECK(names[3] == "D");
}

TEST_CASE("WorkIterator: stays inside the project it was made on", "[iterator]") {
	Project root(3, "Root");
	Project* sub = new Project(3, "Sub");
	root.add(sub);
	sub->add(new WorkUnit("Inside", "coder"));
	root.add(new WorkUnit("Outside", "coder"));

	std::vector<std::string> names = readyUnitNames(sub);
	REQUIRE(names.size() == 1);
	CHECK(names[0] == "Inside");
}

TEST_CASE("WorkIterator: next after the end stays done", "[iterator]") {
	Project root(3, "Root");
	root.add(new WorkUnit("A", "coder"));
	WorkIterator it(&root);

	it.first();
	CHECK_FALSE(it.isDone());
	it.next();
	CHECK(it.isDone());
	it.next();
	CHECK(it.isDone());
	CHECK(it.currentItem() == nullptr);
}
