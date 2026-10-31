
#ifndef WORKUNIT_H
#define WORKUNIT_H

#include <string>

#include "WorkNode.h"

/**
 * @class WorkUnit
 * @brief A single piece of work that one profession can do; the leaf of the work tree.
 *
 * A work unit needs exactly one profession. It moves through the states
 * Ready, Ongoing, Escalated and Completed as employees work on it.
 *
 * @par Design patterns
 * - Composite (Leaf): cannot have children, so add() does nothing useful.
 * - Prototype (ConcretePrototype): clone() copies the unit.
 *
 * @see WorkNode, Project, Team
 */
class WorkUnit : public WorkNode {

private:

	std::string requiredProfession; ///< Profession needed to do this unit.

public:

	/**
	 * @brief Creates a work unit.
	 * @param name Name of the unit.
	 * @param requiredProfession Profession needed. An empty string means no profession is required.
	 */
	WorkUnit(std::string name, std::string requiredProfession = "");

	/** @brief Gets the profession needed to do this unit. */
	std::string getRequiredProfession() const;

	/**
	 * @brief Not supported: a work unit is a leaf and cannot have children.
	 * @param workNode The node that would be added.
	 */
	void add(WorkNode* workNode);

	/**
	 * @brief Makes a copy of this unit.
	 * @return A newly allocated copy. The caller owns it.
	 */
	WorkNode* clone();

	/**
	 * @brief Checks whether this unit has a given name.
	 * @param name The name to compare.
	 * @return true if the names match.
	 */
	bool hasName(std::string name);

};

#endif
