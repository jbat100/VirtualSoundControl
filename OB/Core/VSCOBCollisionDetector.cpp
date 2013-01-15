/*
 *  VSC Stuff
 */

#include "VSCOBCollisionDetector.h"

#include "VSCOB.h"
#include "VSCException.h"
#include "VSCOBApplication.h"
#include "VSCOBScene.h"
#include "VSCOBElement.h"

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

using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;


//MARK: - Collisions Detector

bool VSC::OB::CollisionDetector::checkListener(VSC::Listener::SPtr listener)
{
    CollisionDetector::Listener::SPtr collisionDetectorListener = boost::dynamic_pointer_cast<CollisionDetector::Listener>(listener);
    
    if (collisionDetectorListener)
    {
        return true;
    }
    
    return false;
}

VSC::OB::Collisions VSC::OB::CollisionDetector::getCollisionsForElementPair(Element::SPtr first, Element::SPtr second)
{
    Collisions collisions;
    
    BOOST_FOREACH(Collision::SPtr collision, mCollisions)
    {
        if (collision->getFirstElement() == first && collision->getSecondElement() == second) {
            collisions.push_back(collision);
        }
        if (collision->getFirstElement() == second && collision->getSecondElement() == first) {
            collisions.push_back(collision);
        }
    }
    return collisions;
}

VSC::OB::Collision::SPtr VSC::OB::CollisionDetector::getCollisionsForPersistentManifold(btPersistentManifold* manifold)
{
    BOOST_FOREACH (Collision::SPtr collision, mCollisions)
    {
        if (collision->getPersistentManifold() == manifold)
        {
            return collision;
        }
    }
    
    return Collision::SPtr();
}

void VSC::OB::CollisionDetector::updateCollisions()
{
    // http://bulletphysics.org/mediawiki-1.5.8/index.php/Collision_Callbacks_and_Triggers
    
    Scene::SPtr scene = this->getScene();
    
    /*
     *  Create a local collisions group so that the difference with the previous one can be
     *  computed so as to send the appropriate collision ended callbacks to the listeners.
     */
    
    Collisions localCollisions;
    
	int numManifolds = scene->getDynamicsWorld()->getBulletCollisionWorld()->getDispatcher()->getNumManifolds();
    
    /*
     *  Get current collisions and collision prospects.
     */
    
	for (int i=0; i < numManifolds; i++)
	{
        
        /*
         *  From http://bulletphysics.com/Bullet/BulletFull/classbtPersistentManifold.html
         *
         *  btPersistentManifold is a contact point cache, it stays persistent as long as objects are overlapping 
         *  in the broadphase.
         *
         *  Those contact points are created by the collision narrow phase. The cache can be empty, or hold 1,2,3 
         *  or 4 points. Some collision algorithms (GJK) might only add one point at a time. updates/refreshes old contact 
         *  points, and throw them away if necessary (distance becomes too large) reduces the cache to 4 points, when more 
         *  then 4 points are added, using following rules: the contact point with deepest penetration is always kept, and 
         *  it tries to maximuze the area covered by the points note that some pairs of objects might have more then one 
         *  contact manifold.
         *
         *  MORE THAN ONE MANIFOLD, AAARRRRgghh... Oh well
         */
        
        
		btPersistentManifold* contactManifold = scene->getDynamicsWorld()->getBulletCollisionWorld()->getDispatcher()->getManifoldByIndexInternal(i);
        
        /*
         *  Get existing collision for element pair or create one.
         */
        
        Collision::SPtr collision = this->getCollisionsForPersistentManifold(contactManifold);
        
        /*
        if (rigidBodyA && rigidBodyB)
        {
            Element::SPtr elementA = scene->getElementFactory()->getElementWithRigidBody(rigidBodyA);
            Element::SPtr elementB = scene->getElementFactory()->getElementWithRigidBody(rigidBodyB);
            if (elementA && elementB) collision = this->getCollisionForElementPair(elementA, elementB);
        }
         */
        
        /*
         *  If a collision does not already exist then create one...
         */
        
        if (!collision)
        {
            /*
             *  Map back collision objects to scene elements
             */
            
            btCollisionObject* obA = static_cast<btCollisionObject*>(contactManifold->getBody0());
            btCollisionObject* obB = static_cast<btCollisionObject*>(contactManifold->getBody1());
            
            OgreBulletCollisions::Object* collisionObjectA = scene->getDynamicsWorld()->findObject(obA);
            OgreBulletCollisions::Object* collisionObjectB = scene->getDynamicsWorld()->findObject(obB);
            
            OgreBulletDynamics::RigidBody* rigidBodyA = dynamic_cast<OgreBulletDynamics::RigidBody*>(collisionObjectA);
            OgreBulletDynamics::RigidBody* rigidBodyB = dynamic_cast<OgreBulletDynamics::RigidBody*>(collisionObjectB);
            
            if (rigidBodyA && rigidBodyB)
            {
                Element::SPtr elementA = scene->getElementWithRigidBody(rigidBodyA);
                Element::SPtr elementB = scene->getElementWithRigidBody(rigidBodyB);
                
                BOOST_ASSERT_MSG(elementA && elementB, "Expected elements");
                
                if (elementA && elementB && !elementA->silentCollisions() && !elementB->silentCollisions())
                {
                    collision = Collision::SPtr(new Collision());
                    collision->setFirstElement(elementA);
                    collision->setSecondElement(elementB);
                    collision->setPersistentManifold(contactManifold);
                    collision->setState(Collision::StateCreated);
                    this->addCollision(collision);
                }
            }
            
        }
        
        if (collision)
        {
            localCollisions.push_back(collision);
        }
        
	}
    
    /*
     *  Cycle through current collisions and invalidate the ones which are no longer part
     *  of the current scene (not in localManifolds), after having sent a callback to the
     *  listeners. Using while so that collisions can be removed from mCollisions during
     *  iteration.
     */
    
    Collisions::iterator currentColIt = mCollisions.begin();
    
    while (currentColIt != mCollisions.end()) 
    {
        Collision::SPtr collision = *currentColIt;
        Collisions::iterator it = std::find(localCollisions.begin(), localCollisions.end(), collision);
        
        /*
         *  Collision no longer exists
         */
        
        if (it == localCollisions.end())
        {
            
            if (collision->getState() == Collision::StateClose)
            {
                collision->setState(Collision::StateEnded);
                BOOST_FOREACH (VSC::Listener::WPtr l, this->getListeners())
                {
                    CollisionDetector::Listener::SPtr listener = boost::dynamic_pointer_cast<CollisionDetector::Listener>(l.lock());
                    if (listener) listener->collisionProspectEnded(collision);
                }
            }
            
            else if (collision->getState() == Collision::StateOngoing)
            {
                collision->setState(Collision::StateEnded);
                BOOST_FOREACH (VSC::Listener::WPtr l, this->getListeners())
                {
                    CollisionDetector::Listener::SPtr listener = boost::dynamic_pointer_cast<CollisionDetector::Listener>(l.lock());
                    if (listener) listener->collisionEnded(collision);
                }
            }

            /*
             *  After the callbacks have been made, invalidate the collision and remove from the list
             */
            
            collision->invalidate();
            this->removeCollision(collision);
        }
        
        else
        {
            currentColIt++;
        }
    }
    
    /*
     *  Cycle through the current, still valid collisions (which now contain the new collisions) and 
     *  send update callbacks
     */
    
    BOOST_FOREACH (Collision::SPtr collision, mCollisions)
    {
        BOOST_ASSERT_MSG(collision->getState() != Collision::StateInvalid, "Expected Valid Collision");
        
        /*
         *  Don't assert ended state, the collision could start, end, and restart
         */
        
        // BOOST_ASSERT_MSG(collision->getState() != Collision::StateEnded, "Expected non Ended Collision");
        
        btPersistentManifold* manifold = collision->getPersistentManifold();
        
        // btScalar is defined by: typedef float btScalar
        
        /*
         *  Get minimum distance, of all the manifolds for the collision
         */
        
        btScalar minDistance = std::numeric_limits<btScalar>::max();
        btManifoldPoint deepestPoint;
        
        int numContacts = manifold->getNumContacts();
        for (int j=0; j < numContacts; j++)
        {
            btManifoldPoint pt = manifold->getContactPoint(j);
            if (pt.getDistance() < minDistance)
            {
                minDistance = pt.getDistance();
                deepestPoint = pt;
            }
        }
        
        /*
         *  If the minimum distance is below the threshold we have an ongoing collision
         */
        
        if (minDistance < mDistanceThreshold)
        {
            if (collision->getState() == Collision::StateNone      ||
                collision->getState() == Collision::StateCreated   ||
                collision->getState() == Collision::StateClose     ||
                collision->getState() == Collision::StateEnded) 
            {
                // calculate collision velocity (subtract the object velocity vectors)
                Element::SPtr firstElement = collision->getFirstElement();
                Element::SPtr secondElement = collision->getSecondElement();
                BOOST_ASSERT(firstElement);
                BOOST_ASSERT(secondElement);
                if (firstElement && secondElement)
                {
                    OgreBulletDynamics::RigidBody* firstBody = firstElement->getRigidBody();
                    OgreBulletDynamics::RigidBody* secondBody = secondElement->getRigidBody();
                    BOOST_ASSERT(firstBody);
                    BOOST_ASSERT(secondBody);
                    if (firstBody && secondBody)
                    {
                        Ogre::Vector3 firstVelocity = firstBody->getLinearVelocity();
                        Ogre::Vector3 secondVelocity = secondBody->getLinearVelocity();
                        Ogre::Vector3 relativeVelocity = firstVelocity - secondVelocity;
                        collision->setFirstElementCollisionVelocity(firstVelocity);
                        collision->setSecondElementCollisionVelocity(secondVelocity);
                        collision->setRelativeCollisionVelocity(relativeVelocity);
                    }
                }
                // update state and call listeners
                collision->setState(Collision::StateOngoing);
                BOOST_FOREACH (VSC::Listener::WPtr l, this->getListeners())
                {
                    CollisionDetector::Listener::SPtr listener = boost::dynamic_pointer_cast<CollisionDetector::Listener>(l.lock());
                    if (listener) listener->collisionDetected(collision);
                }
            }
            else if (collision->getState() == Collision::StateOngoing)
            {
                BOOST_FOREACH (VSC::Listener::WPtr l, this->getListeners())
                {
                    CollisionDetector::Listener::SPtr listener = boost::dynamic_pointer_cast<CollisionDetector::Listener>(l.lock());
                    if (listener) listener->collisionUpdated(collision);
                }
            }
        }
        
        else if (minDistance > mDistanceThreshold)
        {
            if (collision->getState() == Collision::StateNone      ||
                collision->getState() == Collision::StateCreated   ||
                collision->getState() == Collision::StateEnded)
            {
                collision->setState(Collision::StateClose);
                BOOST_FOREACH (VSC::Listener::WPtr l, this->getListeners())
                {
                    CollisionDetector::Listener::SPtr listener = boost::dynamic_pointer_cast<CollisionDetector::Listener>(l.lock());
                    if (listener) listener->collisionProspectDetected(collision);
                }
            }
            else if (collision->getState() == Collision::StateOngoing)
            {
                collision->setState(Collision::StateEnded);
                BOOST_FOREACH (VSC::Listener::WPtr l, this->getListeners())
                {
                    CollisionDetector::Listener::SPtr listener = boost::dynamic_pointer_cast<CollisionDetector::Listener>(l.lock());
                    if (listener) listener->collisionEnded(collision);
                }
                collision->setState(Collision::StateClose);
                BOOST_FOREACH (VSC::Listener::WPtr l, this->getListeners())
                {
                    CollisionDetector::Listener::SPtr listener = boost::dynamic_pointer_cast<CollisionDetector::Listener>(l.lock());
                    if (listener) listener->collisionProspectDetected(collision);
                }
            }
            else if (collision->getState() == Collision::StateClose)
            {
                BOOST_FOREACH (VSC::Listener::WPtr l, this->getListeners())
                {
                    CollisionDetector::Listener::SPtr listener = boost::dynamic_pointer_cast<CollisionDetector::Listener>(l.lock());
                    if (listener) listener->collisionProspectUpdated(collision);
                }
            }
        }
    }
}

void VSC::OB::CollisionDetector::addCollision(Collision::SPtr collision)
{
    Collisions::iterator it = std::find(mCollisions.begin(), mCollisions.end(), collision);
    if (it == mCollisions.end()) {
        mCollisions.push_back(collision);
    }
}

void VSC::OB::CollisionDetector::removeCollision(Collision::SPtr collision)
{
    Collisions::iterator it = std::find(mCollisions.begin(), mCollisions.end(), collision);
    if (it != mCollisions.end()) {
        mCollisions.erase(it);
    }
}
