
#ifndef WORKUNIT_H
#define WORKUNIT_H

#include <string>

#include "WorkNode.h"

/** @brief A single piece of work. Leaf of the work tree. */
class WorkUnit : public WorkNode {

private:

	/** Profession needed for this unit. Empty means any. */
	std::string requiredProfession;

public:

	/**
	 * @brief Creates a unit with a name and the profession it needs.
	 * @param name Unit name.
	 * @param requiredProfession Profession it needs.
	 */
	WorkUnit(std::string name, std::string requiredProfession = "");

	/**
	 * @brief Gets the profession this unit needs.
	 * @return The profession needed.
	 */
	std::string getRequiredProfession() const;
	// for gui:
	void setRequiredProfession(std::string profession);

	/**
	 * @brief Does nothing, a unit has no children.
	 * @param workNode Ignored.
	 */
	void add(WorkNode* workNode);

	/**
	 * @brief Makes a copy of the unit with its state.
	 * @return A new copy; the caller owns it.
	 */
	WorkNode* clone();

	/**
	 * @brief True if the name matches this unit's.
	 * @param name Name to look for.
	 * @return True if the name is found.
	 */
	bool hasName(std::string name);

	/**
	 * @brief Returns this unit if the name matches, else nullptr.
	 * @param name Name to find.
	 * @return The node, or nullptr.
	 */
	virtual WorkNode* getNode(std::string name);

};

#endif
