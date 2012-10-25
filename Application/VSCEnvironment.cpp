
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

void VSC::Environment::addCollisionActionChain(IM::CollisionActionChain::SPtr actionChain)
{
    BOOST_ASSERT(actionChain);
    if (!actionChain) return;
    
    IM::CollisionActionChains::iterator it = std::find(mIMCollisionActionChains.begin(), mIMCollisionActionChains.end(), actionChain);
    BOOST_ASSERT(it == mIMCollisionActionChains.end());
    
    if (it == mIMCollisionActionChains.end())
    {
        mIMCollisionActionChains.push_back(actionChain);
    }
}

void VSC::Environment::removeCollisionActionChain(IM::CollisionActionChain::SPtr actionChain)
{
    BOOST_ASSERT(actionChain);
    if (!actionChain) return;
    
    IM::CollisionActionChains::iterator it = std::find(mIMCollisionActionChains.begin(), mIMCollisionActionChains.end(), actionChain);
    BOOST_ASSERT(it != mIMCollisionActionChains.end());
    
    if (it != mIMCollisionActionChains.end())
    {
        mIMCollisionActionChains.erase(it);
    }
}
