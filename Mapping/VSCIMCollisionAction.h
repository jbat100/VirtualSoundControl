//
//  VSCIMAction.h
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_ACTION_H_
#define _VSC_IM_ACTION_H_

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
            
            typedef boost::shared_ptr<Action> SPtr;
            
            virtual void perform(OB::Scene::Element::SPtr element, OB::Scene::Collision::SPtr collision) = 0;
            
        };

        typedef std::vector<CollisionAction::SPtr> CollisionActions;

    }
    
}

#endif // _VSC_IM_ACTION_H_
