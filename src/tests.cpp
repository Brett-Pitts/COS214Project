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