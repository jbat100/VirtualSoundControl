/*
 *  VSC Stuff
 */

#include "VSCOBElement.h"

#include "VSCOB.h"
#include "VSCException.h"
#include "VSCOBApplication.h"
#include "VSCOBScene.h"
#include "VSCOBElementFactory.h"


#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "OgreBulletCollisionsRay.h"
#include "Debug/OgreBulletCollisionsDebugLines.h"

#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletDynamicsConstraint.h"
#include "Constraints/OgreBulletDynamicsPoint2pointConstraint.h" 

#include "btBulletCollisionCommon.h"

#include <boost/foreach.hpp>

#include <limits>

//MARK: - Elements

std::ostream& VSC::OB::operator << (std::ostream& stream, const Element& element)
{
    stream << element.getName() << "_" << element.getIdentifier();
    
    return stream;
}

void VSC::OB::Element::destroy()
{
    Scene::SPtr scene = this->getScene();
    
    if (scene) {
        scene->getDynamicsWorld()->removeObject(mRigidBody);
        delete mRigidBody;
        mRigidBody = 0;
    }

}

bool VSC::OB::Element::isImmobilized(void)
{
    /*
     *  Checks, defensive programming
     */
    
    if (!mImmobilized)
    {
        BOOST_ASSERT(mRigidBody);
        if (mRigidBody)
        {
            btRigidBody* bulletRigidBody = mRigidBody->getBulletRigidBody();
            BOOST_ASSERT(bulletRigidBody);
            if (bulletRigidBody)
            {
                BOOST_ASSERT((~bulletRigidBody->getCollisionFlags()) & btCollisionObject::CF_KINEMATIC_OBJECT);
                //BOOST_ASSERT(bulletRigidBody->getActivationState() == ACTIVE_TAG);
            }
        }
    }
    else
    {
        BOOST_ASSERT(mRigidBody);
        if (mRigidBody)
        {
            btRigidBody* bulletRigidBody = mRigidBody->getBulletRigidBody();
            BOOST_ASSERT(bulletRigidBody);
            if (bulletRigidBody)
            {
                BOOST_ASSERT(bulletRigidBody->getCollisionFlags() & btCollisionObject::CF_KINEMATIC_OBJECT);
                //BOOST_ASSERT(bulletRigidBody->getActivationState() == DISABLE_DEACTIVATION);
            }
        }
    }
    
    return mImmobilized;
}

void VSC::OB::Element::setImmobilized(bool immobilized)
{
    if (immobilized == mImmobilized) return;
    
    mImmobilized = immobilized;
    
    BOOST_ASSERT(mRigidBody);
    if (mRigidBody)
    {
        btRigidBody* bulletRigidBody = mRigidBody->getBulletRigidBody();
        BOOST_ASSERT(bulletRigidBody);
        if (bulletRigidBody)
        {
            if (mImmobilized)
            {
                bulletRigidBody->setCollisionFlags( bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_STATIC_OBJECT);
                bulletRigidBody->setCollisionFlags( bulletRigidBody->getCollisionFlags() | btCollisionObject::CF_KINEMATIC_OBJECT);
                bulletRigidBody->setMassProps(0.0, btVector3(0.0, 0.0, 0.0));
            }
            else
            {
                bulletRigidBody->setCollisionFlags( bulletRigidBody->getCollisionFlags() & (~btCollisionObject::CF_KINEMATIC_OBJECT));
                bulletRigidBody->setCollisionFlags( bulletRigidBody->getCollisionFlags() & (~btCollisionObject::CF_KINEMATIC_OBJECT));
                bulletRigidBody->setMassProps(mMass, btVector3(0.0, 0.0, 0.0));
            }
        }
    }
    
}

void VSC::OB::Element::setMass(Float mass)
{
    BOOST_ASSERT(mass > 0);
    
    if (mass == 0.0)
    {
        this->setImmobilized(true);
    }
    else if (mass > 0.0)
    {
        BOOST_ASSERT(mRigidBody);
        if (mRigidBody)
        {
            btRigidBody* bulletRigidBody = mRigidBody->getBulletRigidBody();
            BOOST_ASSERT(bulletRigidBody);
            if (bulletRigidBody)
            {
                bulletRigidBody->setMassProps(mMass, btVector3(0.0, 0.0, 0.0));
            }
        }
        
        mMass = mass;
    }
}

VSC::Float VSC::OB::Element::getMass(void)
{
    return mMass;
}

Ogre::Vector3 VSC::OB::Element::getPosition(void)
{
    OgreBulletDynamics::RigidBody* body = this->getRigidBody();
    
    BOOST_ASSERT(body);
    if (!body) return Ogre::Vector3::ZERO;
    
    Ogre::SceneNode* node = body->getSceneNode();
    BOOST_ASSERT(node);
    if (!node) return Ogre::Vector3::ZERO;
    
    return node->getPosition();
}

Ogre::Vector3 VSC::OB::Element::getVelocity(void)
{
    OgreBulletDynamics::RigidBody* body = this->getRigidBody();
    
    BOOST_ASSERT(body);
    if (body)
    {
        return body->getLinearVelocity();
    }
    
    return Ogre::Vector3::ZERO;

}



