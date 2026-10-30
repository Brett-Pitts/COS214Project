#include "catch.hpp"

#include "Team.h"
#include "TeamIterator.h"
#include "Department.h"
#include "WorkUnit.h"
#include "EmployeeProfession.h"
#include "ConcreteEmployeeBuilder.h"

static EmployeeComponent* makeEmployee(const std::string& name, const std::string& profession, const std::string& competency) {
	ConcreteEmployeeBuilder builder;
	builder.reset();
	builder.addProfession(profession);
	builder.addCompetency(competency);
	return builder.finish(name);
}

TEST_CASE("Teams are keyed by profession name", "[team]") {
	std::string coder = EmployeeProfession::registerProfession("coder");
	std::string tester = EmployeeProfession::registerProfession("tester");
	CHECK(coder == "coder");
	CHECK(EmployeeProfession::hasProfession("coder"));

	SECTION("Team stores its profession name") {
		Team team(coder);
		CHECK(team.getProfession() == "coder");
		CHECK(Team().getProfession() == "");
	}

	SECTION("addMember accepts matching professions and rejects others") {
		Team team(coder);
		EmployeeComponent* alice = makeEmployee("Alice", "coder", "senior");
		EmployeeComponent* bob = makeEmployee("Bob", "tester", "senior");

		CHECK(team.addMember(alice));
		CHECK_FALSE(team.addMember(bob));
		CHECK_FALSE(team.addMember(nullptr));
		delete bob;
	}

	SECTION("A team with no profession accepts anyone") {
		Team team;
		CHECK(team.addMember(makeEmployee("Cara", "coder", "junior")));
		CHECK(team.addMember(makeEmployee("Dan", "tester", "junior")));
	}

	SECTION("clone keeps the profession and members") {
		Team team(coder);
		team.addMember(makeEmployee("Alice", "coder", "senior"));
		Team* copy = (Team*)team.clone();
		CHECK(copy->getProfession() == "coder");
		delete copy;
	}

	SECTION("assignWork only advances work that needs the team's profession") {
		Team team(coder);
		team.addMember(makeEmployee("Alice", "coder", "senior"));

		WorkUnit match("Match", "coder");
		match.setState("Ready");
		team.assignWork(&match);
		CHECK(match.getStateName() != "Ready");

		WorkUnit mismatch("Mismatch", "tester");
		mismatch.setState("Ready");
		team.assignWork(&mismatch);
		CHECK(mismatch.getStateName() == "Ready");

		WorkUnit unknown("Unknown", "astronaut-not-registered");
		unknown.setState("Ready");
		team.assignWork(&unknown);
		CHECK(unknown.getStateName() == "Ready");
	}

	SECTION("TeamIterator finds teams by profession name") {
		Department root;
		Department* sub = new Department();
		Team* coders = new Team(coder);
		Team* testers = new Team(tester);
		Team* coders2 = new Team(coder);
		root.add(coders);
		root.add(sub);
		sub->add(testers);
		sub->add(coders2);

		TeamIterator coderIter(&root, "coder");
		CHECK_FALSE(coderIter.isDone());
		CHECK(coderIter.currentItem() == coders);
		coderIter.next();
		CHECK(coderIter.currentItem() == coders2);
		coderIter.next();
		CHECK(coderIter.isDone());
		CHECK(coderIter.currentItem() == nullptr);
		coderIter.first();
		CHECK(coderIter.currentItem() == coders);

		TeamIterator testerIter(&root, "tester");
		CHECK(testerIter.currentItem() == testers);

		TeamIterator allIter(&root, "");
		int count = 0;
		for (; !allIter.isDone(); allIter.next()) count++;
		CHECK(count == 3);

		TeamIterator noneIter(&root, "nobody");
		CHECK(noneIter.isDone());
	}
}
