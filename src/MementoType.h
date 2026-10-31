#ifndef MEMENTOTYPE_H
#define MEMENTOTYPE_H

/**
 * @enum MementoType
 * @brief Says what a Memento holds.
 *
 * @see Memento, ApplicationInterface
 */
enum MementoType {
	workflow, ///< The memento holds a work tree (WorkNode).
	company   ///< The memento holds a company tree (CompanyNode).
};


#endif
