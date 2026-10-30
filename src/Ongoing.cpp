#include "Ongoing.h"
#include "Escalated.h"
#include "WorkNode.h"

void Ongoing::handle(WorkNode* workNode){
    workNode->setState(new Escalated());
}
