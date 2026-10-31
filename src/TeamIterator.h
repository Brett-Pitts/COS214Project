#ifndef TEAMITERATOR_H
#define TEAMITERATOR_H

#include <string>
#include <vector>

class CompanyNode;
class Team;

/**
 * @class TeamIterator
 * @brief Walks the company tree and visits the teams of a given profession.
 *
 * The iterator collects the matching teams when it is created. It lets the
 * system find a team for a work unit without knowing how departments are
 * stored.
 *
 * @par Design pattern
 * Iterator (ConcreteIterator) over the company composite. It does not derive
 * from Iterator because it returns Team objects, not work nodes.
 *
 * @see Team, Department, CompanyNode
 */
class TeamIterator {

private:
	std::vector<Team*> teams; ///< The matching teams found in the tree. Not owned.
	int current;              ///< Index of the current team.

	/**
	 * @brief Walks the tree below a node and stores the teams that match.
	 * @param node The node to search from.
	 * @param profession Required profession. An empty string matches every team.
	 */
	void collect(CompanyNode* node, const std::string& profession);

public:
	/**
	 * @brief Creates an iterator and collects the matching teams.
	 * @param root Root of the company tree to search.
	 * @param professionName Profession to match. An empty string matches every team.
	 */
	TeamIterator(CompanyNode* root, std::string professionName = "");

	/** @brief Moves to the first matching team. */
	void first();

	/** @brief Moves to the next matching team. */
	void next();

	/**
	 * @brief Checks whether the iteration has finished.
	 * @return true if there are no more teams.
	 */
	bool isDone();

	/**
	 * @brief Gets the team the iterator is on.
	 * @return The current team, or nullptr if the iterator is done.
	 */
	Team* currentItem();
};

#endif
