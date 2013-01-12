
#include "VSCGlobalApplication.h"
#include "VSCEnvironment.h"
#include "VSCOBScene.h"

#include <boost/assert.hpp>

VSC::Environment::Environment()
{
    
}

void VSC::Environment::setIMCollisionMapper(IM::CollisionMapper::SPtr mapper)
{
    if (mIMCollisionMapper && mOBScene && mOBScene->getCollisionDetector())
    {
        mOBScene->getCollisionDetector()->removeListener(mIMCollisionMapper);
    }
    
    mIMCollisionMapper = mapper;
    
    if (mOBScene && mOBScene->getCollisionDetector())
    {
        mOBScene->getCollisionDetector()->addListener(mIMCollisionMapper);
    }
}

void VSC::Environment::setOBScene(OB::Scene::SPtr scene)
{
    if (mOBScene && mOBScene->getCollisionDetector() && mIMCollisionMapper)
    {
        mOBScene->getCollisionDetector()->removeListener(mIMCollisionMapper);
    }
    
    mOBScene = scene;
    
    if (mOBScene)
    {
        BOOST_ASSERT_MSG(mOBScene->getCollisionDetector(), "Scenes added to Environement should have a collision detector");
        
        if (mOBScene->getCollisionDetector() && mIMCollisionMapper)
        {
            mOBScene->getCollisionDetector()->addListener(mIMCollisionMapper);
        }
    }
}

void VSC::Environment::addCollisionEventChain(IM::CollisionEventChain::SPtr actionChain)
{
    BOOST_ASSERT(actionChain);
    if (!actionChain) return;
    
    IM::CollisionEventChains::iterator it = std::find(mIMCollisionEventChains.begin(), mIMCollisionEventChains.end(), actionChain);
    BOOST_ASSERT(it == mIMCollisionEventChains.end());
    
    if (it == mIMCollisionEventChains.end())
    {
        mIMCollisionEventChains.push_back(actionChain);
    }
}

void VSC::Environment::removeCollisionEventChain(IM::CollisionEventChain::SPtr actionChain)
{
    BOOST_ASSERT(actionChain);
    if (!actionChain) return;
    
    IM::CollisionEventChains::iterator it = std::find(mIMCollisionEventChains.begin(), mIMCollisionEventChains.end(), actionChain);
    BOOST_ASSERT(it != mIMCollisionEventChains.end());
    
    if (it != mIMCollisionEventChains.end())
    {
        mIMCollisionEventChains.erase(it);
    }
}
