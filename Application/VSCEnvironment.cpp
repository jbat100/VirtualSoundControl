
#include "VSCGlobalApplication.h"
#include "VSCEnvironment.h"
#include "VSCOBScene.h"

VSC::Environment::Environment(GlobalApplication::SPtr application)
{
    mGlobalApplication = GlobalApplication::WPtr(application);
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
