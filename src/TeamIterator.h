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

	void collect(CompanyNode* node, int profession);

public:
	TeamIterator(CompanyNode* root, int profession);

	TeamIterator(CompanyNode* root, std::string professionName);

	void first();

	void next();

	bool isDone();

	Team* currentItem();
};

#endif
