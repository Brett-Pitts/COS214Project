#include "TeamIterator.h"
#include "CompanyNode.h"
#include "Team.h"

TeamIterator::TeamIterator(CompanyNode* root, std::string professionName) {
	this->current = 0;
	this->collect(root, professionName);
}
#include <iostream>
void TeamIterator::collect(CompanyNode* node, const std::string& profession) {
	if (node == nullptr) {
		return;
	}

	Team* team = dynamic_cast<Team*>(node);
	if (team != nullptr) {
		std::cout << "Collecting: " << team->getName() << "\n";
		std::cout << team->getProfession() << " == " << profession << '\n';
		std::cout << std::to_string(team->getProfession() == profession) << '\n';
		if (profession.empty() || team->getProfession() == profession) {
			std::cout << "Adding: " << team->getName() << "\n";
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
	return this->current >= (int)this->teams.size();
}

Team* TeamIterator::currentItem() {
	if (this->isDone()) {
		return nullptr;
	}
	return this->teams[this->current];
}
