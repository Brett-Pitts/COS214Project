#ifndef MEMENTO_H
#define MEMENTO_H

#include "MementoType.h"
#include "ApplicationInterface.h"
#include "FileSystem.h"

/**
 * @class Memento
 * @brief A snapshot of the company tree or the work tree.
 *
 * One class covers both kinds of snapshot; MementoType says which one it is.
 * Only ApplicationInterface (the originator) and FileSystem (the caretaker)
 * can change it.
 *
 * @par Design pattern
 * Memento (Memento).
 *
 * @see ApplicationInterface, FileSystem, MementoType
 */
class Memento {
	friend class ApplicationInterface;
	friend class FileSystem;
private:
	void* root;        ///< Root of the saved tree (a CompanyNode* or WorkNode*, depending on type).
	MementoType type;  ///< Says whether root is a workflow or a company.


	/**
	 * @brief Swaps in a new saved tree and type; the old tree isn't deleted.
	 * @param root Root of the new tree.
	 * @param type Kind of tree.
	 */
	void setRoot(void* root, MementoType type);
public:
	/**
	 * @brief Gets the kind of snapshot.
	 * @return The kind of snapshot.
	 */
	MementoType getType() const;

	/**
	 * @brief Gets the saved tree's root as an untyped pointer.
	 * @return The saved root.
	 */
	void* getRoot() const;

	/** @brief Deletes the memento and the tree it holds. */
	~Memento();

	/**
	 * @brief Creates a memento that takes ownership of the tree.
	 * @param root Root of the tree (now owned).
	 * @param type Kind of tree.
	 */
	Memento(void* root, MementoType type);
};

#endif
