#ifndef AGGREGATE_H
#define AGGREGATE_H

class Iterator;

class Aggregate {

public:
	virtual ~Aggregate();

	virtual Iterator* createIterator() = 0;
};

#endif
