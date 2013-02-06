
#include "VSCIMCollisionMapping.h"

#include "VSCOBScene.h"
#include "VSCOBElement.h"
#include "VSCOBCollision.h"

#include <Ogre/Ogre.h>
#include <boost/assert.hpp>

using namespace VSC;
using namespace VSC::IM;
using namespace VSC::OB;

CollisionMapping::CollisionMapping()
{
    this->allowMappingType(MappingTypeCollisionVelocity);
    this->allowMappingType(MappingTypeCollisionDistance);
}

Element_SPtr CollisionMapping::getCollisionEffectee(Collision_SPtr collision, Element_SPtr effector)
{
    BOOST_ASSERT(collision);
    BOOST_ASSERT(effector);
    
    Element::SPtr effectee = Element::SPtr();
    
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

Float CollisionMapping::mappedValue(Collision::SPtr collision, Element::SPtr effector)
{
    BOOST_ASSERT(collision);
    BOOST_ASSERT(effector);
    
    Float rawValue = 0.0;
    
    if (collision && effector)
    {
        MappingType mappingType = this->getMappingType();
        switch (mappingType)
        {
            case MappingTypeConstant:
                break;
                
            case MappingTypeCollisionVelocity:
            {
                const Ogre::Vector3& relativeVelocity = collision->getRelativeCollisionVelocity();
                rawValue = (Float) relativeVelocity.length();
            }
                break;
                
            case MappingTypeCollisionDistance:
            {
                Element::SPtr effectee = this->getCollisionEffectee(collision, effector);
                BOOST_ASSERT(effectee);
                if (effectee)
                {
                    // get distance from collision location to reference point
                }
            }
                
            default:
                break;
        }
        

    }
    
    return this->applyOffsetAndScaleFactor(rawValue);
}


