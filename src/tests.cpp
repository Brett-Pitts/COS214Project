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

TEST_CASE("Mediator: notifyColleagues delivers to every colleague except the sender", "[mediator]") {
	ConcreteWorkMediator mediator;
	WorkUnit sender("A", "coder");
	WorkUnit b("B", "coder");
	WorkUnit c("C", "coder");
	WorkUnit d("D", "coder");
	b.addPrerequisite("A");
	c.addPrerequisite("A");
	sender.addPrerequisite("A");
	d.addPrerequisite("Z");

	mediator.colleagues = { &sender, &b, &c, &d };

	mediator.notifyColleagues(&sender, "A");

	CHECK(b.getPrerequisites().empty());
	CHECK(c.getPrerequisites().empty());
	CHECK(sender.getPrerequisites().size() == 1);
	CHECK(d.getPrerequisites().size() == 1);
}

TEST_CASE("Mediator: handles an empty colleague list", "[mediator]") {
	ConcreteWorkMediator mediator;
	WorkUnit a("A", "coder");

	CHECK_NOTHROW(mediator.notifyColleagues(&a, "A"));
	CHECK_NOTHROW(mediator.notifyColleagues(nullptr, "A"));
}

TEST_CASE("Mediator: a message matching no prerequisite changes nothing", "[mediator]") {
	ConcreteWorkMediator mediator;
	WorkUnit a("A", "coder");
	WorkUnit b("B", "coder");
	b.addPrerequisite("A");
	mediator.colleagues = { &a, &b };

	mediator.notifyColleagues(&a, "Unrelated");

	REQUIRE(b.getPrerequisites().size() == 1);
	CHECK(b.getPrerequisites()[0] == "A");
	CHECK(b.getStateName() == "WaitingForPrerequisites");
}

TEST_CASE("Mediator: clearing the last prerequisite moves a waiting node to Ready", "[mediator]") {
	ConcreteWorkMediator mediator;
	WorkUnit a("A", "coder");
	WorkUnit b("B", "coder");
	b.addPrerequisite("A");
	mediator.colleagues = { &a, &b };

	REQUIRE(b.getStateName() == "WaitingForPrerequisites");
	mediator.notifyColleagues(&a, "A");

	CHECK(b.getPrerequisites().empty());
	CHECK(b.isReady());
}

TEST_CASE("Mediator: a node with several prerequisites stays waiting until all are done", "[mediator]") {
	ConcreteWorkMediator mediator;
	WorkUnit a("A", "coder");
	WorkUnit b("B", "coder");
	WorkUnit c("C", "coder");
	c.addPrerequisite("A");
	c.addPrerequisite("B");
	mediator.colleagues = { &a, &b, &c };

	mediator.notifyColleagues(&a, "A");
	CHECK(c.getPrerequisites().size() == 1);
	CHECK(c.getStateName() == "WaitingForPrerequisites");
	CHECK_FALSE(c.isReady());

	mediator.notifyColleagues(&b, "B");
	CHECK(c.getPrerequisites().empty());
	CHECK(c.isReady());
}

TEST_CASE("Mediator: duplicate prerequisites are all cleared by one notification", "[mediator]") {
	ConcreteWorkMediator mediator;
	WorkUnit a("A", "coder");
	WorkUnit b("B", "coder");
	b.addPrerequisite("A");
	b.addPrerequisite("A");
	mediator.colleagues = { &a, &b };

	mediator.notifyColleagues(&a, "A");

	CHECK(b.getPrerequisites().empty());
	CHECK(b.isReady());
}

TEST_CASE("Mediator: a node that is not waiting does not change state when its prerequisites clear", "[mediator]") {
	ConcreteWorkMediator mediator;
	WorkUnit a("A", "coder");
	WorkUnit b("B", "coder");
	b.addPrerequisite("A");
	b.setState("Ongoing");
	mediator.colleagues = { &a, &b };

	mediator.notifyColleagues(&a, "A");

	CHECK(b.getPrerequisites().empty());
	CHECK(b.isOngoing());
}

TEST_CASE("Mediator: a notification is repeatable and does not re-advance a node", "[mediator]") {
	ConcreteWorkMediator mediator;
	WorkUnit a("A", "coder");
	WorkUnit b("B", "coder");
	b.addPrerequisite("A");
	mediator.colleagues = { &a, &b };

	mediator.notifyColleagues(&a, "A");
	REQUIRE(b.isReady());

	mediator.notifyColleagues(&a, "A");
	CHECK(b.isReady());
	CHECK_FALSE(b.isOngoing());
}

TEST_CASE("Mediator: setMediator wires a node to the mediator", "[mediator]") {
	SpyWorkMediator spy;
	WorkUnit a("A", "coder");
	a.setMediator(&spy);

	SECTION("only the final transition into Completed notifies") {
		a.advanceState();
		CHECK(a.isReady());
		a.advanceState();
		CHECK(a.isOngoing());
		a.advanceState();
		CHECK(a.getStateName() == "Escalated");
		CHECK(spy.calls.empty());

		a.advanceState();
		CHECK(a.isCompleted());
		REQUIRE(spy.calls.size() == 1);
		CHECK(spy.calls[0].first == &a);
		CHECK(spy.calls[0].second == "A");
	}

	SECTION("advancing an already completed node does not notify again") {
		completeUnit(a);
		REQUIRE(a.isCompleted());
		REQUIRE(spy.calls.size() == 1);

		a.advanceState();
		a.advanceState();
		CHECK(spy.calls.size() == 1);
	}

	SECTION("a node waiting on prerequisites that cannot advance does not notify") {
		WorkUnit b("B", "coder");
		b.setMediator(&spy);
		b.addPrerequisite("A");

		b.advanceState();
		CHECK(b.getStateName() == "WaitingForPrerequisites");
		CHECK(spy.calls.empty());
	}

	SECTION("a node with no mediator completes without error") {
		WorkUnit lone("Lone", "coder");
		CHECK_NOTHROW(completeUnit(lone));
		CHECK(lone.isCompleted());
	}

	SECTION("replacing the mediator with nullptr stops notifications") {
		a.setMediator(nullptr);
		completeUnit(a);
		CHECK(a.isCompleted());
		CHECK(spy.calls.empty());
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

	REQUIRE(build.getStateName() == "WaitingForPrerequisites");

	completeUnit(design);

	CHECK(design.isCompleted());
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
	CHECK(b.isCompleted());
	CHECK(c.isReady());

	completeUnit(c);
	CHECK(c.isCompleted());
}

TEST_CASE("Mediator: a diamond dependency only unblocks after both branches complete", "[mediator]") {
	ConcreteWorkMediator mediator;
	WorkUnit root("Root", "coder");
	WorkUnit left("Left", "coder");
	WorkUnit right("Right", "coder");
	WorkUnit join("Join", "coder");
	left.addPrerequisite("Root");
	right.addPrerequisite("Root");
	join.addPrerequisite("Left");
	join.addPrerequisite("Right");
	mediator.colleagues = { &root, &left, &right, &join };
	for (WorkNode* n : mediator.colleagues) n->setMediator(&mediator);

	completeUnit(root);
	CHECK(left.isReady());
	CHECK(right.isReady());
	CHECK(join.getStateName() == "WaitingForPrerequisites");

	completeUnit(left);
	CHECK(join.getPrerequisites().size() == 1);
	CHECK_FALSE(join.isReady());

	completeUnit(right);
	CHECK(join.getPrerequisites().empty());
	CHECK(join.isReady());
}

TEST_CASE("Mediator: independent nodes are unaffected by each other's completion", "[mediator]") {
	ConcreteWorkMediator mediator;
	WorkUnit a("A", "coder");
	WorkUnit b("B", "coder");
	mediator.colleagues = { &a, &b };
	a.setMediator(&mediator);
	b.setMediator(&mediator);

	completeUnit(a);

	CHECK(a.isCompleted());
	CHECK(b.getStateName() == "WaitingForPrerequisites");
	CHECK(b.getPrerequisites().empty());
}

TEST_CASE("Mediator: works with Projects and their child units", "[mediator]") {
	ConcreteWorkMediator mediator;
	Project* project = new Project(3, "Release");
	WorkNode* code = new WorkUnit("Code", "coder");
	WorkNode* test = new WorkUnit("Test", "coder");
	project->add(code);
	project->add(test);
	test->addPrerequisite("Code");

	mediator.colleagues = { code, test };
	code->setMediator(&mediator);
	test->setMediator(&mediator);

	completeUnit(*code);

	CHECK(code->isCompleted());
	CHECK(test->isReady());
	CHECK_FALSE(project->isCompleted());

	delete project;
}