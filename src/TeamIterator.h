#ifndef TEAMITERATOR_H
#define TEAMITERATOR_H

#include <string>
#include <vector>

class CompanyNode;
class Team;

class TeamIterator {

private:
	std::vector<Team*> teams;
	int current;

	void collect(CompanyNode* node, const std::string& profession);

public:
	TeamIterator(CompanyNode* root, std::string professionName = "");

	void first();

	void next();

	bool isDone();

	Team* currentItem();
};

#endif
