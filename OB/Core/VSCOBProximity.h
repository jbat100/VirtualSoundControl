
#ifndef _VSC_OB_PROXIMITY_H_
#define _VSC_OB_PROXIMITY_H_

#include "VSC.h"
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
        
        /*
         *  Collision objects are used for tracking and informing Collision listeners
         */
        
        class Proximity
        {
            
        public:
            
            friend class Scene;
            friend class ProximityDetector;
            
            typedef boost::shared_ptr<Proximity>        SPtr;
            typedef boost::weak_ptr<Proximity>          WPtr;
            
            //typedef std::vector<btPersistentManifold*>  PersistentManifolds;
            
            enum State {
                StateNone = 0,
                StateActive,
                StateInactive,
                StateInvalid
            };
            
            Proximity() {}
            
            Scene_SPtr getScene(void) {return mScene.lock();}
            
            Element_SPtr getFirstElement(void) const {return mFirstElement.lock();}
            Element_SPtr getSecondElement(void) const {return mSecondElement.lock();}
            
            State getState(void) {return mState;}
            
            Float getDistanceThreshold(void) {return mDistanceThreshold;}
            void setDistanceThreshold(Float threshold) {mDistanceThreshold = threshold;}
            
            friend std::ostream& operator << (std::ostream& stream, const Proximity& collision);
            
        protected:
            
            void setScene(Scene_WPtr scene);
            
            void setFirstElement(Element_SPtr element) {mFirstElement = Element_WPtr(element);}
            void setSecondElement(Element_SPtr element) {mSecondElement = Element_WPtr(element);}
            
            void setState(State state) {mState = state;}
            
        private:
            
            Scene_WPtr              mScene;
            
            Element_WPtr            mFirstElement;
            Element_WPtr            mSecondElement;
            
            State                   mState;
            
            Float                   mDistanceThreshold;
            
        };
        
    }
}

#endif //_VSC_OB_PROXIMITY_H_

