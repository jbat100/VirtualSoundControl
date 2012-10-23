
#ifndef _VSC_GLOBAL_APPLICATION_CPP_
#define _VSC_GLOBAL_APPLICATION_CPP_

#include "VSCGlobalApplication.h"

#include <boost/assert.hpp>
#include <boost/thread.hpp>

boost::once_flag globalApplicationSingletonInitilizedFlag = BOOST_ONCE_INIT;
VSC::GlobalApplication::SPtr mGlobalApplicationSingleton;

void VSC::GlobalApplication::InitializeSingletonGlobalApplication()
{
    BOOST_ASSERT(!mGlobalApplicationSingleton);
    mGlobalApplicationSingleton = VSC::GlobalApplication::SPtr (new VSC::GlobalApplication);
    mGlobalApplicationSingleton->init();
}

VSC::GlobalApplication::SPtr VSC::GlobalApplication::singletonGlobalApplication()
{
    boost::call_once(&GlobalApplication::InitializeSingletonGlobalApplication, globalApplicationSingletonInitilizedFlag);
    BOOST_ASSERT(mGlobalApplicationSingleton);
    return mGlobalApplicationSingleton;
}

void VSC::GlobalApplication::init()
{
    
    
}

template<typename EnvironmentSubclass>
VSC::Environment::SPtr VSC::GlobalApplication::createEnvironment(void)
{
    
    EnvironmentSubclass* envSub = new EnvironmentSubclass(shared_from_this());
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

void VSC::GlobalApplication::destroyEnvironment(Environment::SPtr environment)
{
    BOOST_ASSERT(environment);
    
    if(environment)
    {
        Environments::iterator it = std::find(mEnvironments.begin(), mEnvironments.end(), environment);
        BOOST_ASSERT(it != mEnvironments.end());
        if(it != mEnvironments.end())
        {
            environment->shutdown();
            mEnvironments.erase(it);
        }
    }
}

#endif // _VSC_GLOBAL_APPLICATION_CPP_
