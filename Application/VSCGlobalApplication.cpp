
#include "VSCGlobalApplication.h"

#include <boost/assert.hpp>

VSC::GlobalApplication::GlobalApplication()
{
    
}

void VSC::GlobalApplication::init()
{
    
    this->setMIDIOutputManager(VSC::MIDI::OutputManager::singletonManager());
    
    this->internalInit();
}

template<typename OBApplicationSubclass>
void VSC::GlobalApplication::createOBApplication()
{
    BOOST_ASSERT_MSG(!mOBApplication, "VSC::GlobalApplication::createOBApplication() should only be called once!");
    if(!mOBApplication) return;
    
    OBApplicationSubclass* applicationSubclass = new OBApplicationSubclass;
    VSC::OB::Application* application = dynamic_cast<VSC::OB::Application*>(applicationSubclass);
    
    BOOST_ASSERT(application);
    
    if (application) {
        mOBApplication = VSC::OB::Application::SPtr(application);
    }
    
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
        }
    }
}
