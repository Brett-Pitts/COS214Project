#include "WorkIterator.h"
#include "WorkNode.h"
#include "WorkUnit.h"

/**
 * @brief Gets the next node in depth-first order, or nullptr at the end.
 * @param aggregate Root of the walk.
 * @param node Node to move on from.
 * @return The next node, or nullptr at the end.
 */
static WorkNode* nextInTree(WorkNode* aggregate, WorkNode* node) {
	if (node == nullptr) {
		return nullptr;
	}
	WorkNode* child = node->getChild(0);
	if (child != nullptr) {
		return child;
	}
	while (node != aggregate) {
		WorkNode* parent = node->getParent();
		if (parent == nullptr) {
			return nullptr;
		}
		int index = 0;
		while (parent->getChild(index) != nullptr && parent->getChild(index) != node) {
			index++;
		}
		WorkNode* sibling = parent->getChild(index + 1);
		if (sibling != nullptr) {
			return sibling;
		}
		node = parent;
	}
	return nullptr;
}

/**
 * @brief True if the node is a work unit in the Ready state.
 * @param node Node to check.
 * @return True if it is a Ready unit.
 */
static bool isReadyUnit(WorkNode* node) {
	return dynamic_cast<WorkUnit*>(node) != nullptr && node->getStateName() == "Ready";
}

// for gui:
WorkIterator::WorkIterator(WorkNode* aggregate, bool readyOnly) : aggregate(aggregate), current(nullptr), readyOnly(readyOnly) {
}

WorkIterator::~WorkIterator() {
}

void WorkIterator::first() {
	current = aggregate;
	if (current != nullptr && readyOnly && !isReadyUnit(current)) {
		next();
	}
}

void WorkIterator::next() {
	if (current == nullptr) {
		return;
	}
	current = nextInTree(aggregate, current);
	while (current != nullptr && readyOnly && !isReadyUnit(current)) {
		current = nextInTree(aggregate, current);
	}
}

bool WorkIterator::isDone() {
	return current == nullptr;
}

WorkNode* WorkIterator::currentItem() {
	return current;
}
