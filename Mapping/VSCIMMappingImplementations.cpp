
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCIMMappingImplementations.h"

#include "VSCOBCollision.h"
#include "VSCOBElement.h"

#include <Ogre/Ogre.h>

#include <boost/foreach.hpp>

using namespace VSC;
using namespace VSC::IM;
using namespace VSC::OB;

Ogre::Vector3 ReferencePointOwner::defaultReferencePoint = Ogre::Vector3::ZERO;




Float Mapping::ImplementationConstant::mappedValue()
{
    return 0.0;
}

Float Mapping::ImplementationConstant::mappedValue(OB::Collision_SPtr collision, OB::Element_SPtr effector)
{
    return 0.0;
}


Float Mapping::ImplementationCollisionVelocity::mappedValue()
{
    BOOST_ASSERT_MSG(false, "Should be calling collision trigger");
    return 0.0;
}

Float Mapping::ImplementationCollisionVelocity::mappedValue(OB::Collision_SPtr collision, OB::Element_SPtr effector)
{
    BOOST_ASSERT(collision);
    BOOST_ASSERT(effector);
    
    const Ogre::Vector3& relativeVelocity = collision->getRelativeCollisionVelocity();
    return (Float) relativeVelocity.length();
}

Float Mapping::ImplementationCollisionDistance::mappedValue()
{
    BOOST_ASSERT_MSG(false, "Should be calling collision trigger");
    return 0.0;
}

Float Mapping::ImplementationCollisionDistance::mappedValue(OB::Collision_SPtr collision, OB::Element_SPtr effector)
{
    BOOST_ASSERT(collision);
    BOOST_ASSERT(effector);
    
    Element::SPtr effectee = getCollisionEffectee(collision, effector);
    BOOST_ASSERT(effectee);
    if (effectee)
    {
        // get distance from collision location to reference point
    }
    return 0.0;
}

