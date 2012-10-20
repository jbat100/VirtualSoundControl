
#ifndef _VSC_ENVIRONMENT_H_
#define _VSC_ENVIRONMENT_H_

#include "VSCOBScene.h"
#include "VSCIMCollisionMapper.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

namespace VSC {
    
    class GlobalApplication;
    
    class Environment
    {
        
    private:
        
        typedef boost::shared_ptr<GlobalApplication>  GlobalApplication_SPtr;
        typedef boost::weak_ptr<GlobalApplication>    GlobalApplication_WPtr;
        
    public:
        
        friend class GlobalApplication;
        
        typedef boost::shared_ptr<Environment>  SPtr;
        typedef boost::weak_ptr<Environment>    WPtr;
        
        OB::Scene::SPtr             getOBScene(void) {return mOBScene;}
        IM::CollisionMapper::SPtr   getIMCollisionMapper(void) {return mIMCollisionMapper;}
        
        void setIMCollisionMapper(IM::CollisionMapper::SPtr mapper) {mIMCollisionMapper = mapper;}
        void setOBScene(OB::Scene::SPtr scene) {mOBScene = scene;}
        
    protected:
        
        Environment(GlobalApplication_SPtr globalApplication);
        
        void init();
        void shutdown();
        
    private:
        
        OB::Scene::SPtr             mOBScene;
        IM::CollisionMapper::SPtr   mIMCollisionMapper;
        
        GlobalApplication_WPtr      mGlobalApplication;
  
    };
    
    typedef std::vector<Environment::SPtr> Environments;

}

#endif //_VSC_GLOBAL_APPLICATION_H_

