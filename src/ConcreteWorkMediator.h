#ifndef CONCRETEWORKMEDIATOR_H
#define CONCRETEWORKMEDIATOR_H

class ConcreteWorkMediator : WorkMediator {

public:
	vector<WorkNode> colleagues;

	void notifyColleagues(WorkNode node, std::string msg);
};

#endif
