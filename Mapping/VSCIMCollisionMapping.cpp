//
//  VSCCollisionReceiver.cpp
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/21/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#include "VSCIMCollisionMapping.h"

#include "VSCOBScene.h"
#include "VSCOBElement.h"
#include "VSCOBCollision.h"

#include <Ogre/Ogre.h>
#include <boost/assert.hpp>

VSC::Float VSC::IM::CollisionMapping::mappedValue(VSC::OB::Element::SPtr element, OB::Collision::SPtr collision)
{
    Float internalValue = this->internalMappedValue(element, collision);
    
    Float outputValue = (internalValue * mScaleFactor) + mOffset;
    
    if (mTrace)
    {
        std::cout << "CollisionMapping::mappedValue: " << outputValue << " (internal value: " << internalValue << ", ";
        std::cout << "scale factor: " << mScaleFactor << ", offset " << mOffset << ")" << std::endl;
    }
    
    return outputValue;
}

VSC::Float VSC::IM::CollisionVelocityMapping::internalMappedValue(OB::Element::SPtr element, OB::Collision::SPtr collision)
{
    BOOST_ASSERT(collision);
    
    if (collision)
    {
        const Ogre::Vector3& relativeVelocity = collision->getRelativeCollisionVelocity();
        return (Float) relativeVelocity.length();
    }
    
    return 0;
}

VSC::Float VSC::IM::CollisionDistanceMapping::internalMappedValue(OB::Element::SPtr element, OB::Collision::SPtr collision)
{
    return 0;
}
