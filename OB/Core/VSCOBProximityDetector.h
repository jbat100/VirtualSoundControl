
#ifndef _VSC_OB_PROXIMITY_DETECTOR_H_
#define _VSC_OB_PROXIMITY_DETECTOR_H_

#include "VSCOB.h"
#include "VSCBroadcaster.h"
#include "VSCListener.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp> 

#include <map>
#include <string>


namespace VSC {
    
    namespace OB {
            
        class ProximityDetector : public VSC::Broadcaster
        {
            
        public:
            
            friend class Scene;
            
            class Listener : public VSC::Listener
            {
                
            public:
                
                typedef boost::shared_ptr<Listener>    SPtr;
                typedef boost::weak_ptr<Listener>      WPtr;
                
                virtual void proximityStarted(Proximity_SPtr proximity) {}
                virtual void proximityUpdated(Proximity_SPtr proximity) {}
                virtual void proximityEnded(Proximity_SPtr proximity) {}
                
            };
            
            typedef boost::shared_ptr<ProximityDetector>    SPtr;
            typedef boost::weak_ptr<ProximityDetector>      WPtr;
            
            ProximityDetector(Scene_SPtr scene) : mScene(Scene_WPtr(scene)) {}
            
            void startMonitoringProximity(Proximity_SPtr proximity);
            void stopMonitoringProximity(Proximity_SPtr proximity);
            
            const Proximities& getProximities(void) {return mProximities;}
            const Proximities getProximitiesForElementPair(Element_SPtr first, Element_SPtr second);
            
            Scene_SPtr getScene(void) {return mScene.lock();}
            
        protected:
            
            virtual void monitorProximities();
            
            virtual bool checkListener(VSC::Listener::SPtr listener);
            
        private:
            
            Proximities     mProximities;
            
            Scene_WPtr      mScene;
            
        };
        
    }
}

#endif //_VSC_OB_PROXIMITY_DETECTOR_H_

