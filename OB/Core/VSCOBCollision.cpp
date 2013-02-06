/*
 *  VSC Stuff
 */

#include "VSCOBCollision.h"

#include "VSCOB.h"
#include "VSCException.h"
#include "VSCOBElement.h"
#include "VSCOBScene.h"

#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletDynamicsConstraint.h"
#include "Constraints/OgreBulletDynamicsPoint2pointConstraint.h" 

#include "btBulletCollisionCommon.h"

using namespace VSC;
using namespace VSC::OB;

//MARK: - Scene Collision 

void Collision::invalidate()
{
    mFirstElement = Element::SPtr();
    mSecondElement = Element::SPtr();
    
    mState = StateInvalid;
    
    mPersistentManifold = 0;
}

std::ostream& operator << (std::ostream& stream, const Collision& collision)
{
    stream << "Collision with element: " << *collision.getFirstElement() << " and element: " << *collision.getSecondElement();
    stream << " relative velocity: " << collision.getRelativeCollisionVelocity() << std::endl;
    
    return stream;
}

Element_SPtr getCollisionEffectee(Collision_SPtr collision, Element_SPtr effector)
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
            BOOST_ASSERT_MSG(false, "Effector should be one of either collision elements");
        }
    }
    
    return effectee;
}

Element_SPtr getCollisionEffector(Collision_SPtr collision, Element_SPtr effectee)
{
    BOOST_ASSERT(collision);
    BOOST_ASSERT(effectee);
    
    Element::SPtr effector = Element::SPtr();
    
    if (collision && effectee)
    {
        if (effectee == collision->getFirstElement())
        {
            effector = collision->getSecondElement();
        }
        else if (effectee == collision->getSecondElement())
        {
            effector = collision->getFirstElement();
        }
        else
        {
            BOOST_ASSERT_MSG(false, "Effectee should be one of either collision elements");
        }
    }
    
    return effector;
}


