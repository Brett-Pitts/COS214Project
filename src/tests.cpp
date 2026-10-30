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