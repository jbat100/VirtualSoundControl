
#ifndef _VSC_GLOBAL_APPLICATION_H_
#define _VSC_GLOBAL_APPLICATION_H_


#include "VSCOBApplication.h"
#include "VSCIMCollisionMapper.h"
#include "VSCMIDIOutputManager.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

namespace VSC {
    
    class GlobalApplication
    {
        
    public:
        
        typedef boost::shared_ptr<GlobalApplication>  SPtr;
        typedef boost::weak_ptr<GlobalApplication>    WPtr;
        
        GlobalApplication();
        
        void initialize();
        
        MIDI::OutputManager::SPtr   getMIDIOutputManager(void) {return mMIDIOutputManager;}
        OB::Application::SPtr       getOBApplication(void) {return mOBApplication;}
        IM::CollisionMapper::SPtr   getIMCollisionMapper(void) {return mCollisionMapper;}
        
    protected:
        
        void setMIDIOutputManager(MIDI::OutputManager::SPtr manager) {mMIDIOutputManager = manager;}
        void setOBApplication(OB::Application::SPtr application) {mOBApplication = application;}
        void setIMCollisionMapper(IM::CollisionMapper::SPtr mapper) {mIMCollisionMapper = mapper;}
        
    private:
        
        MIDI::OutputManager::SPtr  mMIDIOutputManager;
        OB::Application::SPtr      mOBApplication;
        IM::CollisionMapper::SPtr  mIMCollisionMapper;
  
    };

}

#endif //_VSC_GLOBAL_APPLICATION_H_

