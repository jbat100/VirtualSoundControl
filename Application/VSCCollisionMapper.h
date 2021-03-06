//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_IM_COLLISION_MAPPER_H_
#define _VSC_IM_COLLISION_MAPPER_H_

#include "VSC.h"
#include "VSCBroadcaster.h"
#include "VSCOB.h"
#include "VSCIM.h"
#include "VSCOBCollisionDetector.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>

#include <map>

namespace VSC {
    
    class CollisionMapper : public OB::CollisionDetector::Listener, public VSC::Broadcaster,
    public boost::enable_shared_from_this<CollisionMapper>
    {
        
    public:
        
        typedef boost::shared_ptr<CollisionMapper>    SPtr;
        typedef boost::weak_ptr<CollisionMapper>      WPtr;
        
        /**
         *  Listener interface
         */
        
        class Listener : public VSC::Listener
        {
            
        public:
            
            typedef boost::shared_ptr<CollisionMapper::Listener>  SPtr;
            typedef boost::weak_ptr<CollisionMapper::Listener>    WPtr;
            
            virtual ~Listener() {}
            
            virtual void collisionMapperUpdatedMapping(VSC::CollisionMapper::SPtr collisionMapper) {}
            
        };
        
        /**
         *  Action Chains
         */
        
        void addEventChainForCollisionStarted(IM::EventChain_SPtr eventChain, OB::Element_SPtr element);
        void addEventChainForCollisionEnded(IM::EventChain_SPtr eventChain, OB::Element_SPtr element);
        
        void removeEventChainForCollisionStarted(IM::EventChain_SPtr eventChain, OB::Element_SPtr element);
        void removeEventChainForCollisionEnded(IM::EventChain_SPtr eventChain, OB::Element_SPtr element);
        
        const IM::EventChains& getEventChainsForCollisionStarted(OB::Element_SPtr element);
        const IM::EventChains& getEventChainsForCollisionEnded(OB::Element_SPtr element);
        
        void clearEventChain(IM::EventChain_SPtr eventChain);
        
        /**
         *  CollisionDetector::Listener
         */
        
        virtual void collisionProspectDetected(OB::Collision_SPtr collision);
        virtual void collisionProspectUpdated(OB::Collision_SPtr collision);
        virtual void collisionProspectEnded(OB::Collision_SPtr collision);
        
        virtual void collisionDetected(OB::Collision_SPtr collision);
        virtual void collisionUpdated(OB::Collision_SPtr collision);
        virtual void collisionEnded(OB::Collision_SPtr collision);
        
    private:
        
        void brodcastUpdate(void);
        
        typedef std::map<OB::Element_SPtr, IM::EventChains> ElementEventChainMap;
        
        ElementEventChainMap mCollisionStartedEventChainMap;
        ElementEventChainMap mCollisionEndedEventChainMap;
        
        static const bool mTraceCollisions = true;
        
    };
    
}

#endif // _VSC_IM_COLLISION_MAPPER_H_
