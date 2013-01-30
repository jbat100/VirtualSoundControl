
#include "VSCGlobalApplication.h"
#include "VSCEnvironment.h"
#include "VSCOBScene.h"

#include <boost/foreach.hpp>
#include <boost/assert.hpp>

using namespace VSC;
using namespace VSC::IM;
using namespace VSC::OB;

Environment::Environment()
{
    
}

void Environment::setCollisionMapper(CollisionMapper::SPtr mapper)
{
    if (mCollisionMapper && mScene && mScene->getCollisionDetector())
    {
        mScene->getCollisionDetector()->removeListener(mCollisionMapper);
    }
    
    mCollisionMapper = mapper;
    
    if (mScene && mScene->getCollisionDetector())
    {
        mScene->getCollisionDetector()->addListener(mCollisionMapper);
    }
}

void Environment::setScene(Scene::SPtr scene)
{
    if (mScene && mScene->getCollisionDetector() && mCollisionMapper)
    {
        mScene->getCollisionDetector()->removeListener(mCollisionMapper);
    }
    
    mScene = scene;
    
    if (mScene)
    {
        BOOST_ASSERT_MSG(mScene->getCollisionDetector(), "Scenes added to Environement should have a collision detector");
        
        if (mScene->getCollisionDetector() && mCollisionMapper)
        {
            mScene->getCollisionDetector()->addListener(mCollisionMapper);
        }
    }
}

EventChain::SPtr Environment::createEventChain(void)
{
    EventChain::SPtr eventChain = EventChain::SPtr(new EventChain);
    
    BOOST_ASSERT(eventChain);
    
    if (eventChain)
    {
        std::string username = this->generateUsernameForNewEventChain();
        eventChain->setUsername(username);
        mEventChains.push_back(eventChain);
    }
    
    return eventChain;
    
}

void VSC::Environment::destroyEventChain(EventChain::SPtr eventChain)
{
    BOOST_ASSERT(eventChain);
    if (!eventChain) return;
    
    EventChains::iterator it = std::find(mEventChains.begin(), mEventChains.end(), eventChain);
    BOOST_ASSERT(it != mEventChains.end());
    
    if (it != mEventChains.end())
    {
        mEventChains.erase(it);
    }
}

EventChain::SPtr getEventChainWithUsername(std::string username)
{
    BOOST_FOREACH(EventChain::SPtr eventChain, mEventChains)
    {
        BOOST_ASSERT(eventChain);
        if (eventChain && eventChain->getUsername().compare(username) == 0)
        {
            return eventChain;
        }
    }
    return EventChain::SPtr();
}

std::string Environment::generateUsernameForNewEventChain(void)
{
    static std::string baseName = "Event Chain";
    
    int count = 1;
    while (count < mEventChains.size() + 1)
    {
        std::stringstream usernameStream;
        usernameStream << baseName << " " << count;
        std::string username = usernameStream.str();
        if (!this->getEventChainWithUsername(username))
        {
            return username;
        }
    }
    
    BOOST_ASSERT_MSG(false, "Could not generate user name...");
    
    return "";
}
