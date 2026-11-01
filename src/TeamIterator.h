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
 * The iterator collects the matching teams when it is created and starts on the
 * first one, so it can be used straight away; first() goes back to the start.
 * Departments are searched through getChild(); a Team is never searched any
 * further. The list is a snapshot, so if teams are removed from the tree later
 * the iterator holds dangling pointers. It lets the system find a team for a work
 * unit without knowing how departments are stored.
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
	 * @brief Gathers the matching teams below the node.
	 * @param node Node to search from.
	 * @param profession Required profession ("" matches all).
	 */
	void collect(CompanyNode* node, const std::string& profession);

public:
	/**
	 * @brief Collects the matching teams and moves to the first one.
	 * @param root Root of the company tree.
	 * @param professionName Profession to match ("" matches all).
	 */
	TeamIterator(CompanyNode* root, std::string professionName = "");

	/** @brief Goes back to the first team. */
	void first();

	/** @brief Moves to the next team (check isDone() first). */
	void next();

	/**
	 * @brief True when no teams are left.
	 * @return True when finished.
	 */
	bool isDone();

	/**
	 * @brief Gets the current team, or nullptr when done.
	 * @return The current item, or nullptr when done.
	 */
	Team* currentItem();
};

#endif
