#ifndef CONCRETEWORKMEDIATOR_H
#define CONCRETEWORKMEDIATOR_H

class ConcreteWorkMediator : WorkMediator {

public:
	vector<WorkNode> colleagues;

	void notifyColleagues(WorkNode node, String msg);
};

#endif
