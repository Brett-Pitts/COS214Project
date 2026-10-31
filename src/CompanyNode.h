#ifndef COMPANYNODE_H
#define COMPANYNODE_H

/**
 * @file CompanyNode.h
 * @brief Declares the component of the company composite.
 */

/**
 * @class CompanyNode
 * @brief Common interface for departments and teams in the company structure.
 *
 * @par Design patterns
 * - Composite (Component): Department is the composite and Team is the leaf
 *   (teams hold employees).
 * - Prototype (Prototype): clone() produces a deep copy, used when the
 *   company is saved in a Memento.
 *
 * @see Department
 */
class CompanyNode {


public:
	/**
	 * @brief Adds a child to this node.
	 * @param companyNode The child to add. Ownership passes to this node.
	 */
	virtual void add(CompanyNode* companyNode) = 0;

	/**
	 * @brief Removes a child from this node.
	 * @param companyNode The child to remove.
	 */
	virtual void remove(CompanyNode* companyNode) = 0;

	/**
	 * @brief Gets a child by position.
	 * @param n Zero-based index of the child.
	 * @return The nth child, or nullptr if @p n is out of bounds.
	 */
	virtual CompanyNode* getChild(int n) = 0;

	/**
	 * @brief Makes a deep copy of this node and everything below it.
	 * @return A newly allocated copy. The caller owns it.
	 */
	virtual CompanyNode* clone() = 0;

	/** @brief Virtual destructor so derived nodes are destroyed correctly. */
	virtual ~CompanyNode();
};

#endif
