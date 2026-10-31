
#ifndef WORKUNIT_H
#define WORKUNIT_H

#include <string>

#include "WorkNode.h"

/**
 * @class WorkUnit
 * @brief A single piece of work that one profession can do; the leaf of the work tree.
 *
 * A work unit needs exactly one profession, and its retry limit is fixed at 3. It
 * starts in WaitingForPrerequisites and then moves through Ready, Ongoing,
 * Escalated and Completed as employees work on it.
 *
 * @par Design patterns
 * - Composite (Leaf): cannot have children, so add() does nothing.
 * - Prototype (ConcretePrototype): clone() copies the unit.
 *
 * @see WorkNode, Project, Team
 */
class WorkUnit : public WorkNode {

private:

	std::string requiredProfession; ///< Profession needed to do this unit.

public:
	/** @brief Virtual destructor. */
	virtual ~WorkUnit();

	/**
	 * @brief Creates a work unit in the WaitingForPrerequisites state.
	 * @param name Name of the unit.
	 * @param requiredProfession Profession needed to do the unit. An empty string means a team of any profession may take it.
	 */
	WorkUnit(std::string name, std::string requiredProfession = "");

	/** @brief Gets the profession needed to do this unit. */
	std::string getRequiredProfession() const;

	/**
	 * @brief Does nothing: a work unit is a leaf and cannot have children.
	 * @param workNode Ignored. It is not added and not deleted.
	 */
	void add(WorkNode* workNode);

	/**
	 * @brief Makes a copy of this unit.
	 *
	 * Only the name and the required profession are copied. The copy starts in
	 * WaitingForPrerequisites, without prerequisites, retry count, mediator or
	 * assignee.
	 *
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
