
#include "VSCOgreInputAdapter.h"

void VSCOgreBulletCocoaInputAdapter::addOgreBulletInputListener(VSCOgreInputListener* listener)
{
    mInputListeners.insert(listener);
}
void VSCOgreBulletCocoaInputAdapter::removeOgreBulletInputListener(VSCOgreInputListener* listener)
{
    mInputListeners.erase(listener);
}


