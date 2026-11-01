#include "Ready.h"
#include "Ongoing.h"
#include "WorkNode.h"

void Ready::handle(WorkNode* workNode){
    workNode->setState(new Ongoing());
}
