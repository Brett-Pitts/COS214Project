#include "WorkIterator.h"
#include "WorkNode.h"
#include "WorkUnit.h"

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

static bool isReadyUnit(WorkNode* node) {
	return dynamic_cast<WorkUnit*>(node) != nullptr && node->getStateName() == "Ready";
}

WorkIterator::WorkIterator(WorkNode* aggregate) : aggregate(aggregate), current(nullptr) {
}

WorkIterator::~WorkIterator() {
}

void WorkIterator::first() {
	current = aggregate;
	if (current != nullptr && !isReadyUnit(current)) {
		next();
	}
}

void WorkIterator::next() {
	if (current == nullptr) {
		return;
	}
	current = nextInTree(aggregate, current);
	while (current != nullptr && !isReadyUnit(current)) {
		current = nextInTree(aggregate, current);
	}
}

bool WorkIterator::isDone() {
	return current == nullptr;
}

WorkNode* WorkIterator::currentItem() {
	return current;
}
