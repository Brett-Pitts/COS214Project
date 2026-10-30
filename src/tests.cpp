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
	CHECK((child1 == child2 && !(child1 != nullptr ^ child2 != nullptr)));

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
		CHECK((child1 == child2 && !(child1 != nullptr ^ child2 != nullptr)));



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
	Team team(EmployeeProfession::registerProfession("coder"));
	team.addMember(director.makeIntern("Ian", "coder"));
	WorkUnit unit("login screen", "coder");
	unit.advanceState();

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
		a.advanceState();
		CHECK(spy.calls.empty());
	}

	SECTION("completing notifies once with the node's name") {
		completeUnit(a);
		a.advanceState();
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

TEST_CASE("WorkNode: a new node starts waiting with no parent or prerequisites", "[worknode]") {
	WorkUnit unit("Unit", "coder");

	CHECK(unit.getName() == "Unit");
	CHECK(unit.getStateName() == "WaitingForPrerequisites");
	CHECK(unit.getParent() == nullptr);
	CHECK(unit.getPrerequisites().empty());
	CHECK(unit.prerequisitesCompleted());
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

	unit.advanceState();
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

	CHECK_FALSE(unit.prerequisitesCompleted());
	unit.advanceState();
	CHECK(unit.getStateName() == "WaitingForPrerequisites");

	unit.update("Other");
	CHECK(unit.prerequisitesCompleted());
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

TEST_CASE("States: a project completes only when all of its children have", "[state]") {
	Project* project = new Project(3, "Project");
	WorkNode* first = new WorkUnit("First", "coder");
	WorkNode* second = new WorkUnit("Second", "coder");
	project->add(first);
	project->add(second);

	SECTION("the project completes when the last child completes") {
		completeUnit(*first);
		CHECK_FALSE(project->isCompleted());

		completeUnit(*second);
		CHECK(project->isCompleted());
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
	unit.setState("Failed");

	for (int i = 0; i < 3; i++) {
		CHECK(unit.hasRetriesLeft());
		unit.reset();
		CHECK(unit.isReady());
	}

	CHECK_FALSE(unit.hasRetriesLeft());
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
	CHECK(senior->compareCompetency(EmployeeCompetency::SENIOR));
	CHECK(intern->compareCompetency(EmployeeCompetency::INTERN));
	CHECK_FALSE(intern->compareCompetency(EmployeeCompetency::JUNIOR));
	CHECK_FALSE(plain.compareCompetency(EmployeeCompetency::INTERN));

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
	unit.advanceState();

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
	CHECK(copy->compareCompetency(EmployeeCompetency::SENIOR));
	CHECK(bossCopy->getKind() == "Manager");
	CHECK(bossCopy->getName() == "Bea");

	delete senior;
	delete copy;
	delete bossCopy;
}

TEST_CASE("Team: members are notified when a colleague does work", "[team]") {
	Team team;
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
	Team team(EmployeeProfession::registerProfession("coder"));

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
		Team open;
		CHECK(open.addMember(new Employee("Pat")));
	}
}

TEST_CASE("Team: assignWork ignores work it cannot use", "[team]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	Team team(EmployeeProfession::registerProfession("coder"));
	team.addMember(director.makeSenior("Sam", "coder"));
	EmployeeProfession::registerProfession("designer");

	SECTION("work for another profession") {
		WorkUnit unit("Logo", "designer");
		unit.advanceState();
		quietAssign(team, &unit);
		CHECK(unit.isReady());
	}

	SECTION("work needing a profession nobody registered") {
		WorkUnit unit("Moon", "astronaut");
		unit.advanceState();
		quietAssign(team, &unit);
		CHECK(unit.isReady());
	}

	SECTION("a team with no members") {
		Team empty;
		WorkUnit unit("Task", "coder");
		unit.advanceState();
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
	Team team;

	SECTION("a senior takes work from Ready to Completed") {
		team.addMember(director.makeSenior("Sam", "coder"));
		WorkUnit unit("Task", "coder");
		unit.advanceState();

		quietAssign(team, &unit);
		CHECK(unit.isCompleted());
	}

	SECTION("work nobody can do is retried and then failed") {
		team.addMember(new Employee("Eve"));
		WorkUnit unit("Task");
		unit.advanceState();

		for (int i = 0; i < 3; i++) {
			quietAssign(team, &unit);
			CHECK(unit.isReady());
		}
		quietAssign(team, &unit);
		CHECK(unit.getStateName() == "Failed");
	}
}

TEST_CASE("Team: clone copies the profession and members", "[team]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	Team original(EmployeeProfession::registerProfession("coder"));
	original.addMember(director.makeSenior("Sam", "coder"));

	Team* copy = dynamic_cast<Team*>(original.clone());
	REQUIRE(copy != nullptr);
	CHECK(copy != &original);
	CHECK(copy->getProfession() == "coder");

	WorkUnit unit("Task", "coder");
	unit.advanceState();
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
	EmployeeCompetency* competency = new EmployeeCompetency("senior");
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
	Team team;
	EmployeeComponent* sam = director.makeSenior("Sam", "coder");
	EmployeeComponent* ann = director.makeSenior("Ann", "coder");
	team.addMember(sam);
	team.addMember(ann);
	WorkUnit unit("Task", "coder");

	std::string output = capturedDoWork(*sam, &unit);

	CHECK(output.find("Sam received") == std::string::npos);
	CHECK(output.find("Ann received: Sam worked on Task") != std::string::npos);
}