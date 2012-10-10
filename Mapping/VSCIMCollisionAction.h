//
//  VSCIMAction.h
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_COLLISION_ACTION_H_
#define _VSC_IM_COLLISION_ACTION_H_

#include "VSC.h"
#include "VSCOBScene.h"

#include <boost/shared_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace IM {
        
        /*
         *  A very abstract class to provide an interface for performable actions
         */
        
        class CollisionAction {
            
        public:
            
            typedef boost::shared_ptr<CollisionAction> SPtr;
            
            CollisionAction() : mPerformed(false) {}
            
            /*
             *  perform() will call the protected internalPerform after the specified delay, on the same
             *  thread (using boost asio ?)
             */
            
            void perform(OB::Scene::Element::SPtr element, OB::Scene::Collision::SPtr collision, Float delay = 0.0);
            
            bool hasBeenPerformed(void) {return mPerformed;}
            
        protected:
            
            virtual void internalPerform(OB::Scene::Element::SPtr element, OB::Scene::Collision::SPtr collision) = 0;
            
            void setPerformed(bool performed) {mPerformed = performed;}
            
        private:
            
            bool mPerformed;
            
        };

        typedef std::vector<CollisionAction::SPtr> CollisionActions;

    }
    
}

#endif // _VSC_IM_COLLISION_ACTION_H_
