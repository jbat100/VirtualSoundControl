
#include "VSCOgreInputAdapter.h"

#include <boost/assert.hpp>

VSCOgreInputAdapter::VSCOgreInputAdapter(void) :
mNormalizedCoordinates(false)
{
    
}

void VSCOgreInputAdapter::addInputListener(VSCOgreInputListener* listener)
{
    BOOST_ASSERT_MSG(listener, "Expected listener");
    mInputListeners.insert(listener);
}
void VSCOgreInputAdapter::removeInputListener(VSCOgreInputListener* listener)
{
    mInputListeners.erase(listener);
}


