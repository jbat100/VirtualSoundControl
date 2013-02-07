
#ifndef _VSC_ENVIRONMENT_H_
#define _VSC_ENVIRONMENT_H_

#include "VSCOBScene.h"
#include "VSCCollisionMapper.h"
#include "VSCIMEventChain.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <vector>

namespace VSC
{
    
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
        
        OB::Scene_SPtr getScene(void) {return mScene;}
        CollisionMapper::SPtr getCollisionMapper(void) {return mCollisionMapper;}
        
        void setCollisionMapper(CollisionMapper::SPtr mapper);
        void setScene(OB::Scene_SPtr scene);
        
        IM::EventChain::SPtr createEventChain(void);
        void destroyEventChain(IM::EventChain::SPtr actionChain);
        
        const IM::EventChains& getEventChains(void) {return mEventChains;}
        IM::EventChain::SPtr getEventChainWithUsername(std::string username);
        
    protected:
        
        Environment();
        
        void init() {mState = StateValid; internalInit();}
        void shutdown() {internalShutdown(); mState = StateInvalid;}
        
        virtual void internalInit() {}
        virtual void internalShutdown() {}
        
    private:
        
        std::string generateUsernameForNewEventChain(void);
        
        OB::Scene_SPtr              mScene;
        
        CollisionMapper::SPtr   mCollisionMapper;
        
        IM::EventChains             mEventChains;
        
        State                       mState;
  
    };
    
    typedef std::vector<Environment::SPtr> Environments;

}

#endif //_VSC_ENVIRONMENT_H_

