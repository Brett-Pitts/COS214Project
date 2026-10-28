#ifndef ITERATOR_H
#define ITERATOR_H

class WorkNode;


class Iterator {


public:
	WorkNode* currentItem();

	void first();

	bool isDone();

	void next();
};

#endif
