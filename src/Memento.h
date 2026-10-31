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
	 * @brief Replaces the saved tree.
	 * @param root Root of the new tree.
	 * @param type What kind of tree it is.
	 */
	void setRoot(void* root, MementoType type);
public:
	/** @brief Gets the kind of snapshot. */
	MementoType getType() const;

	/** @brief Gets the root of the saved tree as an untyped pointer. */
	void* getRoot() const;

	/** @brief Destroys the memento and the tree it holds. */
	~Memento();

	/**
	 * @brief Creates a memento.
	 * @param root Root of the tree to keep. Ownership passes to the memento.
	 * @param type What kind of tree it is.
	 */
	Memento(void* root, MementoType type);
};

#endif
