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


//MARK: - Scene Collision 

void VSC::OB::Collision::invalidate()
{
    mFirstElement = Element::SPtr();
    mSecondElement = Element::SPtr();
    
    mState = StateInvalid;
    
    mPersistentManifold = 0;
}

std::ostream& VSC::OB::operator << (std::ostream& stream, const Collision& collision)
{
    stream << "Collision with element: " << *collision.getFirstElement() << " and element: " << *collision.getSecondElement();
    stream << " relative velocity: " << collision.getRelativeCollisionVelocity() << std::endl;
    
    return stream;
}
