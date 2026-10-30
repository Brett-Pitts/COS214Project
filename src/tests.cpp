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
#include "Employee.h"
#include "Manager.h"
#include "EmployeeDetail.h"
#include "EmployeeCompetency.h"
#include "Department.h"
#include "TeamIterator.h"
#include "Iterator.h"
#include "WorkIterator.h"
#include <vector>
#include <string>

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

TEST_CASE("Decorator: profession registry", "[decorator]") {
	EmployeeProfession::registerProfession("coder");
	CHECK(EmployeeProfession::hasProfession("coder"));
	CHECK_FALSE(EmployeeProfession::hasProfession("plumber"));
	CHECK(EmployeeProfession::registerProfession("coder") == "coder");
}

TEST_CASE("Decorator: competency levels", "[decorator]") {
	CHECK(EmployeeCompetency::getCompetencyLevel("intern") == EmployeeCompetency::INTERN);
	CHECK(EmployeeCompetency::getCompetencyLevel("junior") == EmployeeCompetency::JUNIOR);
	CHECK(EmployeeCompetency::getCompetencyLevel("senior") == EmployeeCompetency::SENIOR);
	CHECK(EmployeeCompetency::getCompetencyLevel("wizard") == -1);
}

TEST_CASE("Decorator: plain employee has no profession", "[decorator]") {
	Employee alice("Alice");
	CHECK(alice.getName() == "Alice");
	CHECK(alice.getKind() == "Employee");
	CHECK_FALSE(alice.compareProfession("coder"));
}

TEST_CASE("Decorator: wrapped employee keeps its name and gains the profession", "[decorator]") {
	EmployeeComponent* bob = new EmployeeProfession("coder");
	bob->wrap(new EmployeeCompetency("junior"));
	bob->wrap(new Employee("Bob"));
	CHECK(bob->getName() == "Bob");
	CHECK(bob->compareProfession("coder"));
	CHECK_FALSE(bob->compareProfession("artist"));
	CHECK(bob->compareCompetency(EmployeeCompetency::INTERN));
	CHECK(bob->compareCompetency(EmployeeCompetency::JUNIOR));
	CHECK_FALSE(bob->compareCompetency(EmployeeCompetency::SENIOR));
	delete bob;
}

TEST_CASE("Decorator: one employee can hold two professions", "[decorator]") {
	EmployeeComponent* carol = new EmployeeProfession("coder");
	carol->wrap(new EmployeeProfession("artist"));
	carol->wrap(new Employee("Carol"));
	CHECK(carol->compareProfession("coder"));
	CHECK(carol->compareProfession("artist"));
	CHECK(carol->getName() == "Carol");
	delete carol;
}

TEST_CASE("Decorator: clone copies the whole chain", "[decorator]") {
	EmployeeComponent* eve = new EmployeeProfession("coder");
	eve->wrap(new EmployeeCompetency("senior"));
	eve->wrap(new Employee("Eve"));
	EmployeeComponent* copy = eve->clone();
	delete eve;
	CHECK(copy->getName() == "Eve");
	CHECK(copy->compareProfession("coder"));
	CHECK(copy->compareCompetency(EmployeeCompetency::SENIOR));
	delete copy;
}

TEST_CASE("Decorator: manager is an employee", "[decorator]") {
	Manager mia("Mia");
	CHECK(mia.getName() == "Mia");
	CHECK(mia.getKind() == "Manager");
	EmployeeComponent* copy = mia.clone();
	CHECK(copy->getKind() == "Manager");
	delete copy;
}

TEST_CASE("Decorator: builder produces a decorated employee", "[decorator]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	EmployeeComponent* ian = director.makeIntern("Ian", "coder");
	CHECK(ian->getName() == "Ian");
	CHECK(ian->compareProfession("coder"));
	CHECK(ian->compareCompetency(EmployeeCompetency::INTERN));
	CHECK_FALSE(ian->compareCompetency(EmployeeCompetency::JUNIOR));
	delete ian;
}

TEST_CASE("Composite: team only accepts members with its profession", "[composite]") {
	ConcreteEmployeeBuilder builder;
	EmployeeDirector director(&builder);
	Team team(EmployeeProfession::registerProfession("coder"));
	CHECK(team.addMember(director.makeIntern("Ian", "coder")));
	EmployeeComponent* artist = director.makeIntern("Ann", "artist");
	CHECK_FALSE(team.addMember(artist));
	CHECK_FALSE(team.addMember(nullptr));
	delete artist;
}

TEST_CASE("Composite: departments hold teams and teams are found by profession", "[composite]") {
	Department company;
	Department* dev = new Department();
	Team* coders = new Team(EmployeeProfession::registerProfession("coder"));
	Team* artists = new Team(EmployeeProfession::registerProfession("artist"));
	dev->add(coders);
	dev->add(artists);
	company.add(dev);

	CHECK(company.getChild(0) == dev);
	CHECK(dev->getChild(0) == coders);
	CHECK(dev->getChild(1) == artists);
	CHECK(dev->getChild(2) == nullptr);

	TeamIterator it(&company, "artist");
	it.first();
	REQUIRE_FALSE(it.isDone());
	CHECK(it.currentItem() == artists);
	it.next();
	CHECK(it.isDone());
}

static WorkUnit* readyUnit(std::string name) {
	WorkUnit* u = new WorkUnit(name, "coder");
	u->setState("Ready");
	return u;
}

static std::vector<std::string> readyNames(WorkNode* root) {
	std::vector<std::string> names;
	Iterator* it = root->createIterator();
	for (it->first(); !it->isDone(); it->next()) {
		names.push_back(it->currentItem()->getName());
	}
	delete it;
	return names;
}

TEST_CASE("Iterator: empty project yields nothing", "[iterator]") {
	Project root(3, "Root");
	CHECK(readyNames(&root).empty());
}

TEST_CASE("Iterator: only ready work units are returned, in order", "[iterator]") {
	Project root(3, "Root");
	root.add(readyUnit("A"));
	WorkUnit* b = new WorkUnit("B", "coder");
	b->setState("Completed");
	root.add(b);
	root.add(readyUnit("C"));
	std::vector<std::string> names = readyNames(&root);
	REQUIRE(names.size() == 2);
	CHECK(names[0] == "A");
	CHECK(names[1] == "C");
}

TEST_CASE("Iterator: nested projects are walked depth first", "[iterator]") {
	Project root(3, "Root");
	Project* sub = new Project(3, "Sub");
	root.add(readyUnit("A"));
	sub->add(readyUnit("B"));
	sub->add(readyUnit("C"));
	root.add(sub);
	root.add(readyUnit("D"));
	std::vector<std::string> names = readyNames(&root);
	REQUIRE(names.size() == 4);
	CHECK(names[0] == "A");
	CHECK(names[1] == "B");
	CHECK(names[2] == "C");
	CHECK(names[3] == "D");
}

TEST_CASE("Iterator: stays inside the node it was created on", "[iterator]") {
	Project root(3, "Root");
	Project* sub = new Project(3, "Sub");
	sub->add(readyUnit("Inside"));
	root.add(sub);
	root.add(readyUnit("Outside"));
	std::vector<std::string> names = readyNames(sub);
	REQUIRE(names.size() == 1);
	CHECK(names[0] == "Inside");
}

TEST_CASE("Iterator: next past the end stays done", "[iterator]") {
	Project root(3, "Root");
	root.add(readyUnit("A"));
	Iterator* it = root.createIterator();
	it->first();
	it->next();
	CHECK(it->isDone());
	it->next();
	CHECK(it->isDone());
	CHECK(it->currentItem() == nullptr);
	delete it;
}
