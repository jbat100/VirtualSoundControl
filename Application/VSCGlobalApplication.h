
#ifndef _VSC_GLOBAL_APPLICATION_H_
#define _VSC_GLOBAL_APPLICATION_H_

#include "VSCEnvironment.h"
#include "VSCOBApplication.h"
#include "VSCIMCollisionMapper.h"
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
        
        GlobalApplication();
        
        void init();
        
        template<typename OBApplicationSubclass>
        void createOBApplication();
        
        MIDI::OutputManager::SPtr   getMIDIOutputManager(void) {return mMIDIOutputManager;}
        OB::Application::SPtr       getOBApplication(void) {return mOBApplication;}
        
        const Environments&         getEnvironments(void) {return mEnvironments;}
        
        template<typename EnvironmentSubclass>
        Environment::SPtr createEnvironment(void);
        
        void destroyEnvironment(Environment::SPtr environment);
        
    protected:
        
        virtual void internalInit(void) {}
        
        void setMIDIOutputManager(MIDI::OutputManager::SPtr manager) {mMIDIOutputManager = manager;}
        void setOBApplication(OB::Application::SPtr application) {mOBApplication = application;}
        
    private:
        
        MIDI::OutputManager::SPtr   mMIDIOutputManager;
        OB::Application::SPtr       mOBApplication;
        
        Environments                mEnvironments;
  
    };

}

#endif //_VSC_GLOBAL_APPLICATION_H_

