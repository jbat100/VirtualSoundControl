
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCIMMappingImplementations.h"

#include "VSCIM.h"
#include "VSCOBCollision.h"
#include "VSCOBElement.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

using namespace VSC;
using namespace VSC::IM;
using namespace VSC::OB;

Ogre::Vector3 ReferencePointOwner::defaultReferencePoint = Ogre::Vector3::ZERO;


Float Mapping::ImplementationConstant::mappedValue(Trigger trigger, TriggerPayload::SPtr payload)
{
    return 0.0;
}

/*
 
 // good values for collision velocity
 
 velMapping->setOffset(0.0);
 velMapping->setScaleFactor(3.0);
 
 */

Float Mapping::ImplementationCollisionVelocity::mappedValue(Trigger trigger, TriggerPayload::SPtr payload)
{
    BOOST_ASSERT(payload);
    TriggerCollisionPayload::SPtr collisionPayload = boost::dynamic_pointer_cast<TriggerCollisionPayload>(payload);
    BOOST_ASSERT_MSG(collisionPayload, "Wrong payload type");
    if (collisionPayload)
    {
        Collision::SPtr collision = collisionPayload->collision;
        BOOST_ASSERT(collision);
        if (collision)
        {
            const Ogre::Vector3& relativeVelocity = collision->getRelativeCollisionVelocity();
            return (Float) relativeVelocity.length();
        }
    }
    
    return 0.0;
}

Float Mapping::ImplementationCollisionDistance::mappedValue(Trigger trigger, TriggerPayload::SPtr payload)
{
    BOOST_ASSERT(payload);
    TriggerCollisionPayload::SPtr collisionPayload = boost::dynamic_pointer_cast<TriggerCollisionPayload>(payload);
    BOOST_ASSERT_MSG(collisionPayload, "Wrong payload type");
    
    if (collisionPayload)
    {
        Collision::SPtr collision = collisionPayload->collision;
        Element::SPtr effector = collisionPayload->effector;
        BOOST_ASSERT(collision);
        BOOST_ASSERT(effector);
        if (collision)
        {
            Element::SPtr effectee = getCollisionEffectee(collision, effector);
            BOOST_ASSERT(effectee);
            if (effectee)
            {
                // get distance from collision location to reference point
            }
        }
    }
    
    return 0.0;
}

