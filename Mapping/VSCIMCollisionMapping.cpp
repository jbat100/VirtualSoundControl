//
//  VSCCollisionReceiver.cpp
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCIMCollisionMapping.h"

#include <Ogre/Ogre.h>
#include <boost/assert.hpp>

VSC::Float VSC::IM::CollisionMapping::mappedValue(VSC::OB::Scene::Element::SPtr element, OB::Scene::Collision::SPtr collision)
{
    Float internalValue = this->internalMappedValue(element, collision);
    
    return (internalValue * mScaleFactor) + mOffset;
}

VSC::Float VSC::IM::CollisionVelocityMapping::internalMappedValue(OB::Scene::Element::SPtr element, OB::Scene::Collision::SPtr collision)
{
    BOOST_ASSERT(collision);
    
    if (collision)
    {
        const Ogre::Vector3& relativeVelocity = collision->getRelativeCollisionVelocity();
        return (Float) relativeVelocity.length();
    }
    
    return 0;
}

VSC::Float VSC::IM::CollisionDistanceMapping::internalMappedValue(OB::Scene::Element::SPtr element, OB::Scene::Collision::SPtr collision)
{
    return 0;
}
