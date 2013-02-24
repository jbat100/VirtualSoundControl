
#ifndef _VSC_GLOBAL_APPLICATION_H_
#define _VSC_GLOBAL_APPLICATION_H_

#include "VSCEnvironment.h"
#include "VSCOBApplication.h"
#include "VSCCollisionMapper.h"
#include "VSCMIDIOutputManager.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

namespace VSC {
    
    class GlobalApplication : public boost::enable_shared_from_this<GlobalApplication>
    {
        
    public:
        
        typedef boost::shared_ptr<GlobalApplication>  SPtr;
        typedef boost::weak_ptr<GlobalApplication>    WPtr;
        
        static GlobalApplication::SPtr singletonGlobalApplication(void);
        
        ~GlobalApplication();
        
        const Environments& getEnvironments(void) {return mEnvironments;}
        
        template<typename EnvironmentSubclass>
        Environment::SPtr createEnvironment(void);
        
        void destroyEnvironment(Environment::SPtr environment);
        
    protected:
        
        GlobalApplication();
        
        void init();
        
    private:
        
        static GlobalApplication::SPtr mGlobalApplicationSingleton;
        static void InitializeSingletonGlobalApplication();
        
        Environments mEnvironments;
  
    };

}

template<typename EnvironmentSubclass>
VSC::Environment::SPtr VSC::GlobalApplication::createEnvironment(void)
{
    
    EnvironmentSubclass* envSub = new EnvironmentSubclass();
    Environment* env = dynamic_cast<Environment*>(envSub);
    
    BOOST_ASSERT(env);
    
    if (env)
    {
        Environment::SPtr environment = Environment::SPtr(env);
        environment->init();
        mEnvironments.push_back(environment);
        return environment;
    }
    else
    {
        delete envSub;
    }
    
    return Environment::SPtr();
}

#endif //_VSC_GLOBAL_APPLICATION_H_

