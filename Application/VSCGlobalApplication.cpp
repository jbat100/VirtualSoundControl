

#include "VSCGlobalApplication.h"

#include <boost/assert.hpp>
#include <boost/thread.hpp>

boost::once_flag globalApplicationSingletonInitilizedFlag = BOOST_ONCE_INIT;
VSC::GlobalApplication::SPtr VSC::GlobalApplication::mGlobalApplicationSingleton;

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

VSC::GlobalApplication::GlobalApplication()
{
    
}

VSC::GlobalApplication::~GlobalApplication()
{
    
}

void VSC::GlobalApplication::init()
{
    
    
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


