
#include "VSCOgreInputAdapter.h"

VSCOgreInputAdapter::VSCOgreInputAdapter(void) :
mNormalizedCoordinates(false)
{
    
}

void VSCOgreInputAdapter::addInputListener(VSCOgreInputListener* listener)
{
    mInputListeners.insert(listener);
}
void VSCOgreInputAdapter::removeInputListener(VSCOgreInputListener* listener)
{
    mInputListeners.erase(listener);
}


