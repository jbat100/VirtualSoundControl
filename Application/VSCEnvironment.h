
#ifndef _VSC_ENVIRONMENT_H_
#define _VSC_ENVIRONMENT_H_

#include "VSCOBScene.h"
#include "VSCIMCollisionMapper.h"
#include "VSCIMCollisionEventChain.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <vector>

namespace VSC {
    
    class GlobalApplication;
    
    class Environment : public VSC::Broadcaster, public boost::enable_shared_from_this<Environment>
    {
        
    private:
        
        typedef boost::shared_ptr<GlobalApplication>  GlobalApplication_SPtr;
        typedef boost::weak_ptr<GlobalApplication>    GlobalApplication_WPtr;
        
    public:
        
        friend class GlobalApplication;
        
        typedef boost::shared_ptr<Environment>  SPtr;
        typedef boost::weak_ptr<Environment>    WPtr;
        
        class Listener : public VSC::Listener
        {
            
        public:
            
            typedef boost::shared_ptr<Environment::Listener>  SPtr;
            typedef boost::weak_ptr<Environment::Listener>    WPtr;
            
            virtual ~Listener() {}
            
            virtual void environmentChangedScene(VSC::Environment::SPtr environment);
        
        };
        
        enum State
        {
            StateNone = 0,
            StateValid,
            StateInvalid
        };
        
        State getState(void) {return mState;}
        
        OB::Scene_SPtr             getOBScene(void) {return mOBScene;}
        IM::CollisionMapper::SPtr   getIMCollisionMapper(void) {return mIMCollisionMapper;}
        
        void setIMCollisionMapper(IM::CollisionMapper::SPtr mapper);
        void setOBScene(OB::Scene_SPtr scene);
        
        void addCollisionEventChain(IM::CollisionEventChain::SPtr actionChain);
        void removeCollisionEventChain(IM::CollisionEventChain::SPtr actionChain);
        
        const IM::CollisionEventChains& getCollisionEventChains(void) {return mIMCollisionEventChains;}
        
    protected:
        
        Environment();
        
        void init() {mState = StateValid; internalInit();}
        void shutdown() {internalShutdown(); mState = StateInvalid;}
        
        virtual void internalInit() {}
        virtual void internalShutdown() {}
        
    private:
        
        OB::Scene_SPtr             mOBScene;
        IM::CollisionMapper::SPtr   mIMCollisionMapper;

        IM::CollisionEventChains    mIMCollisionEventChains;
        
        State                       mState;
  
    };
    
    typedef std::vector<Environment::SPtr> Environments;

}

#endif //_VSC_GLOBAL_APPLICATION_H_

