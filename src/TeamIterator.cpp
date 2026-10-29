#include "TeamIterator.h"
#include "CompanyNode.h"
#include "Team.h"
#include "EmployeeProfession.h"

TeamIterator::TeamIterator(CompanyNode* root, int profession) {
	this->current = 0;
	this->collect(root, profession);
}

TeamIterator::TeamIterator(CompanyNode* root, std::string professionName) {
	this->current = 0;
	if (professionName == "") {
		this->collect(root, -1);
		return;
	}
	int profession = EmployeeProfession::getProfessionId(professionName);
	if (profession >= 0) {
		this->collect(root, profession);
	}
}

void TeamIterator::collect(CompanyNode* node, int profession) {
	if (node == nullptr) {
		return;
	}

	Team* team = dynamic_cast<Team*>(node);
	if (team != nullptr) {
		if (profession < 0 || team->getProfession() == profession) {
			this->teams.push_back(team);
		}
		return;
	}

	int i = 0;
	CompanyNode* child = node->getChild(i);
	while (child != nullptr) {
		this->collect(child, profession);
		i++;
		child = node->getChild(i);
	}
}

void TeamIterator::first() {
	this->current = 0;
}

void TeamIterator::next() {
	this->current++;
}

bool TeamIterator::isDone() {
	return this->current >= (int) this->teams.size();
}

Team* TeamIterator::currentItem() {
	if (this->isDone()) {
		return nullptr;
	}
	return this->teams[this->current];
}
