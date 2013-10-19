
#include "VSCBroadcaster.h"

void VSC::Broadcaster::addListener(Listener_SPtr listener)
{
    Listeners::iterator it = std::find(mListeners.begin(), mListeners.end(), Listener_WPtr(listener));
    
    BOOST_ASSERT(it == mListeners.end());
    
    if (it == mListeners.end())
    {
        mListeners.push_back(Listener::WPtr(listener));
    }
    
}

void VSC::Broadcaster::removeListener(Listener_SPtr listener)
{
    Listeners::iterator it = std::find(mListeners.begin(), mListeners.end(), Listener_WPtr(listener));
    
    BOOST_ASSERT(it != mListeners.end());
    
    if (it != mListeners.end())
    {
        mListeners.erase(it);
    }
}