
#include "VSCIMCollisionMapping.h"

#include "VSCOBScene.h"
#include "VSCOBElement.h"
#include "VSCOBCollision.h"

#include <Ogre/Ogre.h>
#include <boost/assert.hpp>

VSC::OB::Element_SPtr VSC::IM::CollisionMapping::getCollisionEffectee(OB::Collision_SPtr collision, OB::Element_SPtr effector)
{
    BOOST_ASSERT(collision);
    BOOST_ASSERT(effector);
    
    OB::Element::SPtr effectee = OB::Element::SPtr();
    
    if (collision && effector)
    {
        if (effector == collision->getFirstElement())
        {
            effectee = collision->getSecondElement();
        }
        else if (effector == collision->getSecondElement())
        {
            effectee = collision->getFirstElement();
        }
        else
        {
            BOOST_ASSERT_MSG(false, "Effector should be collision object");
        }
    }
    
    return effectee;
}

VSC::Float VSC::IM::CollisionVelocityMapping::mappedValue(OB::Collision::SPtr collision, OB::Element::SPtr effector)
{
    BOOST_ASSERT(collision);
    
    Float rawValue = 0.0;
    
    if (collision)
    {
        const Ogre::Vector3& relativeVelocity = collision->getRelativeCollisionVelocity();
        rawValue = (Float) relativeVelocity.length();
    }
    
    return this->applyOffsetAndScaleFactor(rawValue);
}


VSC::Float VSC::IM::CollisionDistanceMapping::mappedValue(OB::Collision::SPtr collision, OB::Element::SPtr effector)
{
    BOOST_ASSERT(collision);
    BOOST_ASSERT(effector);
    
    Float rawValue = 0.0;
    
    OB::Element::SPtr effectee = this->getCollisionEffectee(collision, effector);
    BOOST_ASSERT(effectee);
    
    if (effectee)
    {
        // get distance from collision location to reference point
    }
    
    return this->applyOffsetAndScaleFactor(rawValue);
}
