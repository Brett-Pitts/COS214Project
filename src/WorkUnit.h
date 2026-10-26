#ifndef WORKUNIT_H
#define WORKUNIT_H

#include <string>

using namespace std;

class WorkUnit {

private:
	string name;
	int requiredProfession;

public:
	WorkUnit(string name, int requiredProfession = -1) : name(name), requiredProfession(requiredProfession) {}
	string getName() const { return name; }
	int getRequiredProfession() const { return requiredProfession; }
};

#endif
