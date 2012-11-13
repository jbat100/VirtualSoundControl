/*
 *  VSC Stuff
 */

#include "VSCOB.h"
#include "VSCException.h"
#include "VSCOBApplication.h"
#include "VSCOBScene.h"
#include "VSCOBBasicSceneElementFactory.h"

/*
 *  OgreBullet Stuff
 */
#include "OgreBulletCollisionsShape.h"
#include "Shapes/OgreBulletCollisionsBoxShape.h"
#include "Shapes/OgreBulletCollisionsSphereShape.h"
#include "Shapes/OgreBulletCollisionsConeShape.h"
#include "Shapes/OgreBulletCollisionsCylinderShape.h"
#include "Shapes/OgreBulletCollisionsTriangleShape.h"
#include "Shapes/OgreBulletCollisionsStaticPlaneShape.h"
#include "Shapes/OgreBulletCollisionsCompoundShape.h"
#include "Shapes/OgreBulletCollisionsMultiSphereShape.h"
#include "Shapes/OgreBulletCollisionsConvexHullShape.h"
#include "Shapes/OgreBulletCollisionsMinkowskiSumShape.h"
#include "Shapes/OgreBulletCollisionsTrimeshShape.h"

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
using namespace OIS;

/*
 *  GLOBAL VARIABLES
 */

// -------------------------------------------------------------------------
const Ogre::ColourValue g_minLightColour(0.2, 0.1, 0.0);
const Ogre::ColourValue g_maxLightColour(0.5, 0.3, 0.1);
// -------------------------------------------------------------------------


//MARK: - Elements

void VSC::OB::Scene::Element::destroy()
{
    Scene::SPtr scene = this->getScene().lock();
    
    if (scene) {
        scene->getDynamicsWorld()->removeObject(mRigidBody);
        delete mRigidBody;
        mRigidBody = 0;
    }

}

//MARK: - Scene Element Factory

//MARK: - Scene Collision 

void VSC::OB::Scene::Collision::invalidate()
{
    mFirstElement = Scene::Element::SPtr();
    mSecondElement = Scene::Element::SPtr();
    
    mState = StateInvalid;
    
    mPersistentManifold = 0;
}

std::ostream& VSC::OB::operator << (std::ostream& stream, const Scene::Collision& collision)
{
    stream << "Collision with element: " << *collision.getFirstElement() << " and element: " << *collision.getSecondElement() << std::endl;
    
    return stream;
}

std::ostream& VSC::OB::operator << (std::ostream& stream, const Scene::Element& element)
{
    stream << element.getName() << "_" << element.getIdentifier();
    
    return stream;
}

//MARK: - Collisions Detector

bool VSC::OB::Scene::CollisionDetector::checkListener(VSC::Listener::SPtr listener)
{
    CollisionDetector::Listener::SPtr collisionDetectorListener = boost::dynamic_pointer_cast<CollisionDetector::Listener>(listener);
    
    if (collisionDetectorListener)
    {
        return true;
    }
    
    return false;
}

VSC::OB::Scene::Collisions VSC::OB::Scene::CollisionDetector::getCollisionsForElementPair(Scene::Element::SPtr first,
                                                                                       Scene::Element::SPtr second)
{
    Collisions collisions;
    
    BOOST_FOREACH(Scene::Collision::SPtr collision, mCollisions)
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

VSC::OB::Scene::Collision::SPtr VSC::OB::Scene::CollisionDetector::getCollisionsForPersistentManifold(btPersistentManifold* manifold)
{
    BOOST_FOREACH (Scene::Collision::SPtr collision, mCollisions)
    {
        if (collision->getPersistentManifold() == manifold)
        {
            return collision;
        }
    }
    
    return Collision::SPtr();
}

void VSC::OB::Scene::CollisionDetector::updateCollisions()
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
            Scene::Element::SPtr elementA = scene->getElementFactory()->getElementWithRigidBody(rigidBodyA);
            Scene::Element::SPtr elementB = scene->getElementFactory()->getElementWithRigidBody(rigidBodyB);
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
                Scene::Element::SPtr elementA = scene->getElementWithRigidBody(rigidBodyA);
                Scene::Element::SPtr elementB = scene->getElementWithRigidBody(rigidBodyB);
                
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

void VSC::OB::Scene::CollisionDetector::addCollision(Collision::SPtr collision)
{
    Collisions::iterator it = std::find(mCollisions.begin(), mCollisions.end(), collision);
    if (it == mCollisions.end()) {
        mCollisions.push_back(collision);
    }
}

void VSC::OB::Scene::CollisionDetector::removeCollision(Collision::SPtr collision)
{
    Collisions::iterator it = std::find(mCollisions.begin(), mCollisions.end(), collision);
    if (it != mCollisions.end()) {
        mCollisions.erase(it);
    }
}

//MARK: - Scene Listeners

bool VSC::OB::Scene::checkListener(VSC::Listener::SPtr listener)
{
    Scene::Listener::SPtr sceneListener = boost::dynamic_pointer_cast<Scene::Listener>(listener);
    
    if (sceneListener)
    {
        return true;
    }
    
    return false;
}


//MARK: - Basic Constructor which does abolutely nothing interesting at all

VSC::OB::Scene::Scene() :
mSceneManager(0),
mDynamicsWorld(0),
mPaused(false),
mDebugLines(0)
{

}

void VSC::OB::Scene::createSceneManager(void)
{
    
    BOOST_ASSERT(Application::singletonApplication()->getOgreRoot());
    if(!Application::singletonApplication()->getOgreRoot()) return;
    
    // Create the SceneManager, in this case a generic one
    mSceneManager = Application::singletonApplication()->getOgreRoot()->createSceneManager(ST_GENERIC, "ExampleSMInstance");
    BOOST_ASSERT(mSceneManager);
}

void VSC::OB::Scene::createDebugLines(void)
{
    BOOST_ASSERT(!mDebugLines);
    if (mDebugLines) return;
    
    BOOST_ASSERT(this->getSceneManager());
    if (!this->getSceneManager()) return;
    
    mDebugLines = new OgreBulletCollisions::DebugLines();
    this->getSceneManager()->getRootSceneNode()->createChildSceneNode()->attachObject(mDebugLines);
}

void VSC::OB::Scene::init()
{
    this->createSceneManager();
    if(!this->getSceneManager()) return;
    
    this->createDebugLines();
    BOOST_ASSERT(this->getDebugLines());
    
    /*
     *  A bit of OGRE reading ...
     *  - Ogre::Root http://www.ogre3d.org/docs/api/html/classOgre_1_1Root.html
     *  - Ogre::RenderWindow http://www.ogre3d.org/docs/api/html/classOgre_1_1RenderWindow.html
     */
    
    Ogre::Root* root = Application::singletonApplication()->getOgreRoot();
    Ogre::SceneManager* sceneManager = this->getSceneManager();
    
    BOOST_ASSERT(root);
    BOOST_ASSERT(sceneManager);
    if (!(root && sceneManager)) return;

#ifndef _DEBUG // Create shadows if not debug mode
    
    // Should we set the shadow technique once and for all? http://www.ogre3d.org/docs/manual/manual_70.html
    
    mCurrentShadowTechnique = SHADOWTYPE_TEXTURE_ADDITIVE;

    /**
     *  QUESTION: Where does mSceneMgr get set to something other than 0? ANSWER: In the subclasses (Demos) of course.
     *  NOTE: Ogre::SceneManager is an absolutely massive class http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneManager.html
     */
    
	sceneManager->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));

	Ogre::PixelFormat pxlFmt = Ogre::PF_L8;
    
	if (root->getRenderSystem()->getCapabilities()->hasCapability(RSC_TEXTURE_FLOAT))
	{
		const bool isOpenGL = (Ogre::Root::getSingleton().getRenderSystem()->getName().find("GL") != Ogre::String::npos);
        
        // GL performs much better if you pick half-float format
		if (isOpenGL)
			pxlFmt = Ogre::PF_FLOAT16_R;
		else
			pxlFmt = Ogre::PF_FLOAT32_R;
        // D3D is the opposite - if you ask for PF_FLOAT16_R you
		// get an integer format instead! You can ask for PF_FLOAT16_GR
		// but the precision doesn't work well
	}
    
	if (pxlFmt != Ogre::PF_L8)
	{
		String CUSTOM_CASTER_MATERIAL("Ogre/DepthShadowmap/Caster/Float");
		String CUSTOM_RECEIVER_MATERIAL("Ogre/DepthShadowmap/Receiver/Float");
		sceneManager->setShadowTextureSelfShadow(true);
		sceneManager->setShadowTextureCasterMaterial(CUSTOM_CASTER_MATERIAL);
		sceneManager->setShadowTextureReceiverMaterial(CUSTOM_RECEIVER_MATERIAL + "/PCF");
	}

	/**
     *  Set the shadow camera setup to something which looks pretty complicated
     *  http://www.ogre3d.org/docs/api/html/classOgre_1_1LiSPSMShadowCameraSetup.html
     */
	
	Ogre::LiSPSMShadowCameraSetup *mLiSPSMSetup = new Ogre::LiSPSMShadowCameraSetup();
	mLiSPSMSetup->setUseAggressiveFocusRegion(true);
	//mLiSPSMSetup->setUseSimpleOptimalAdjust(true);
	mLiSPSMSetup->setOptimalAdjustFactor(1.1f);
	sceneManager->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(mLiSPSMSetup));

	sceneManager->setShadowTechnique(mCurrentShadowTechnique);
	if (root->getRenderSystem()->getCapabilities()->hasCapability(RSC_HWRENDER_TO_TEXTURE))
	{
		// In D3D, use a 1024x1024 shadow texture
		sceneManager->setShadowTextureSettings(2048, 2, pxlFmt);
	}
	else
	{
		// Use 512x512 texture in GL since we can't go higher than the window res
		sceneManager->setShadowTextureSettings(512, 2, pxlFmt);
	}
#endif // _DEBUG
    
    this->setCollisionDetector(CollisionDetector::SPtr(new CollisionDetector(shared_from_this())));
    
    /**
     *  Nothing is enabled by default...
     */
    
    mPaused = false;
    mDrawWireFrame = false;
    mDrawAabb = false;
    mDrawFeaturesText = false;
    mDrawContactPoints = false;
    mDisableBulletLCP = false;
    mEnableCCD = false;
    mDrawText = false;
    
    mNoDeactivation = false;
    mNoHelpText = false;
    mProfileTimings = false;
    mEnableSatComparison = false;
 
    this->setupLights();
    
    this->initWorld();
    
    this->setupFactory();
    
    Application::singletonApplication()->getOgreRoot()->addFrameListener(this);
    
    this->internalInit();

}

void VSC::OB::Scene::internalInit()
{
    this->getElementFactory()->addGround();
}

void VSC::OB::Scene::shutdown ()
{
    Application::singletonApplication()->getOgreRoot()->removeFrameListener(this);
    
    this->internalShutdown();
    
    /*
     *  It is important to reset the element factory before destroying the scene manager
     *  as it will use it when performing the reset.
     */
    
    this->destroyAllElements();
    
    if (mDynamicsWorld) {
        if (mDynamicsWorld->getDebugDrawer()) {
            delete mDynamicsWorld->getDebugDrawer();
            mDynamicsWorld->setDebugDrawer(0);
        }
        mDynamicsWorld->setDebugDrawer(0);
        delete mDynamicsWorld;
        mDynamicsWorld = 0;
    }
    
    if (mSceneManager) {
        /*
         *  Displays should be destroyed BEFORE their scenes so that the cameras
         *  created through the scene manager can be destroyed through the scene
         *  manager
         */
        mSceneManager = 0;
    }
    
    if (mDebugLines) {
        delete mDebugLines;
        mDebugLines = 0;
    }
    
}

void VSC::OB::Scene::internalShutdown()
{
    
}



void VSC::OB::Scene::setupFactory()
{
    mElementFactory = ElementFactory::SPtr((ElementFactory*) new BasicSceneElementFactory(shared_from_this()));
}

void VSC::OB::Scene::setupLights()
{
    
    Ogre::SceneManager* sceneManager = this->getSceneManager();
    BOOST_ASSERT(sceneManager);
    if (!sceneManager) return;
    
    // Set ambient light
    sceneManager->setAmbientLight(ColourValue(0.4, 0.4, 0.4));
    
    Ogre::Light* l = 0;
    
	// Fixed light, dim
    l = sceneManager->createLight("Sunlight");
 	l->setPosition(0.0, 30.5, 0.0);
 	l->setCastShadows(false);
 	l->setType(Ogre::Light::LT_POINT);
 	l->setDiffuseColour(g_minLightColour);
 	l->setSpecularColour(0.2, 0.2, 0.2);
 	l->setAttenuation(8000, 1, 0.0005, 0);
    mLightMap.insert(LightMap::value_type("Sunlight", l));

	// Point light, movable, reddish
	l = sceneManager->createLight("Spot1");
	l->setType(Light::LT_SPOTLIGHT);
	l->setPosition(100.0, 80.5,-10.0);
	l->setSpotlightRange(Degree(30), Degree(50));
	Ogre::Vector3 dir;
	dir = -l->getPosition();
	dir.normalise();
	l->setDirection(dir);
	l->setDiffuseColour(0.70, 0.70, 0.72);
	l->setSpecularColour(0.1, 0.1, 0.3);
    mLightMap.insert(LightMap::value_type("Spot1", l));

	l = sceneManager->createLight("Spot2");
	l->setType(Light::LT_SPOTLIGHT);
	l->setPosition(-100.0, 80.5, 10.0);
	l->setSpotlightRange(Degree(30), Degree(50));
	dir = -l->getPosition();
	dir.normalise();
	l->setDirection(dir);
	l->setDiffuseColour(0.72, 0.70, 0.70);
	l->setSpecularColour(0.3, 0.1, 0.1);
    mLightMap.insert(LightMap::value_type("Spot2", l));

}
 


void VSC::OB::Scene::setCollisionDetector(CollisionDetector::SPtr detector)
{
    mCollisionDetector = detector;
}

void VSC::OB::Scene::registerElement(Element::SPtr element, std::string name, int identifier)
{
    element->setName(name);
    element->setIdentifier(identifier);
    mElements.push_back(element);
}

void VSC::OB::Scene::destroyAllElements(void)
{
    BOOST_FOREACH (Element::SPtr element, mElements)
    {
        element->destroy();
    }
    
    mElements.clear();
}

void VSC::OB::Scene::destroyElement(Scene::Element::SPtr element)
{
    element->destroy();
    
    Elements::iterator sit = std::find(mElements.begin(), mElements.end(), element);
    if (sit != mElements.end())
    {
        mElements.erase(sit);
    }
}

VSC::OB::Scene::Element::SPtr VSC::OB::Scene::getElementWithRigidBody(OgreBulletDynamics::RigidBody* rigidBody)
{
    BOOST_FOREACH (Scene::Element::SPtr element, mElements)
    {
        if (element->getRigidBody() == rigidBody)
        {
            return element;
        }
    }
    
    return Scene::Element::SPtr();
}


VSC::OB::Scene::Element::SPtr VSC::OB::Scene::getElementAtDisplayCoordinate(Display::SPtr display,
                                                                            const Ogre::Vector2& p,
                                                                            Ogre::Vector3& ip,
                                                                            Ogre::Ray& r)
{
    BOOST_ASSERT(display);
    if (!display) return Element::SPtr();
    
    Ogre::Camera* camera = display->getCamera();
    BOOST_ASSERT(camera);
    if (!camera) return Element::SPtr();
    
    r = camera->getCameraToViewportRay(p.x, p.y);
    CollisionClosestRayResultCallback callback(r, mDynamicsWorld, camera->getFarClipDistance());
    mDynamicsWorld->launchRay(callback);
    
    if (callback.doesCollide())
    {
        OgreBulletDynamics::RigidBody* body = static_cast <OgreBulletDynamics::RigidBody*>(callback.getCollidedObject());
        ip = callback.getCollisionPoint();
        if (body)
        {
            return this->getElementWithRigidBody(body);
        }
    }

    
    return Element::SPtr();
}

// -------------------------------------------------------------------------
bool VSC::OB::Scene::frameStarted(const Ogre::FrameEvent& evt)
{
    Real elapsedTime = evt.timeSinceLastFrame;
    
    if (mTraceFrame) std::cout << "VSC::OB::Scene::frameStarted, elapsed time " << elapsedTime << std::endl;
    
    if (!mPaused || mDoOneStep)
    {
        mDynamicsWorld->stepSimulation(elapsedTime);
    }
    
    if (this->getCollisionDetector()) {
        this->getCollisionDetector()->updateCollisions();
    }

    mDoOneStep = false;

    return true;
}

void VSC::OB::Scene::drawWireFrame(bool draw)
{
    mDrawWireFrame = draw;
    mDynamicsWorld->getDebugDrawer()->setDrawWireframe(draw);
    mDynamicsWorld->setShowDebugShapes(draw);
}

void VSC::OB::Scene::drawAabb(bool draw)
{
    mDrawAabb = draw;
    mDynamicsWorld->getDebugDrawer()->setDrawAabb(draw);
}

void VSC::OB::Scene::drawText(bool draw)
{
    mDrawText = draw;
    mDynamicsWorld->getDebugDrawer()->setDrawText(draw);
}

void VSC::OB::Scene::drawFeaturesText(bool draw)
{
    mDrawFeaturesText = draw;
    mDynamicsWorld->getDebugDrawer()->setDrawFeaturesText(draw);
}

void VSC::OB::Scene::drawContactPoints(bool draw)
{
    mDrawContactPoints = draw;
    mDynamicsWorld->getDebugDrawer()->setDrawContactPoints(draw);
    mDynamicsWorld->setShowDebugContactPoints(draw);
}

void VSC::OB::Scene::enableBulletLCP(bool enable)
{
    mDisableBulletLCP = !enable;
    mDynamicsWorld->getDebugDrawer()->setDisableBulletLCP(mDisableBulletLCP);
}

void VSC::OB::Scene::enableCCD(bool enable)
{
    mEnableCCD = enable;
    mDynamicsWorld->getDebugDrawer()->setEnableCCD(mEnableCCD);
}

// -------------------------------------------------------------------------
bool VSC::OB::Scene::frameEnded(const Ogre::FrameEvent& evt)
{
    Real elapsedTime = evt.timeSinceLastFrame;
    
    if (mTraceFrame) std::cout << "VSC::OB::Scene::frameEnded, elapsed time " << elapsedTime << std::endl;

    /*
    if (mNoDeactivation)
    {
        debugDrawer->setNoDeactivation(!debugDrawer->doesNoDeactivation());
        mNoDeactivation = false;
    }
    if (mNoHelpText)
    {
        debugDrawer->setNoHelpText(!debugDrawer->doesNoHelpText());
        mNoHelpText = false;
    }
    if (mProfileTimings)
    {
        debugDrawer->setProfileTimings(!debugDrawer->doesProfileTimings());
        mProfileTimings = false;
    }
    if (mEnableSatComparison)
    {
        debugDrawer->setEnableSatComparison(!debugDrawer->doesEnableSatComparison());
        mEnableSatComparison = false;
    }
     */
    
    return true;
}

bool VSC::OB::Scene::checkIfEnoughPlaceToAddObjectForDisplay(Display::SPtr display, float maxDist)
{
    Ogre::Vector3 pickPos;
    Ogre::Ray rayTo;
    
    Ogre::Vector2 coord(0.5, 0.5);
    Scene::Element::SPtr element = this->getElementAtDisplayCoordinate(display, coord, pickPos, rayTo);
    
    if (element)
    {
        OgreBulletDynamics::RigidBody *body = element->getRigidBody();
        
        if (body)
        {
            if ((pickPos - display->getCamera()->getDerivedPosition ()).length () < maxDist) return false;
        }
    }
    
    return true;
}

// -------------------------------------------------------------------------
void VSC::OB::Scene::initWorld(const Ogre::Vector3 &gravityVector, const Ogre::AxisAlignedBox &bounds)
{
    Ogre::SceneManager* sceneManager = this->getSceneManager();
    
    BOOST_ASSERT(sceneManager);
    if (!sceneManager) return;
    
    // Start Bullet
    mDynamicsWorld = new OgreBulletDynamics::DynamicsWorld(sceneManager, bounds, gravityVector, true, true, 10000);

    // add Debug info display tool
    OgreBulletCollisions::DebugDrawer *debugDrawer = new OgreBulletCollisions::DebugDrawer();

    mDynamicsWorld->setDebugDrawer(debugDrawer);

    Ogre::SceneNode *node = sceneManager->getRootSceneNode()->createChildSceneNode("DebugDrawer", Ogre::Vector3::ZERO);
    node->attachObject (static_cast<SimpleRenderable*>(debugDrawer));
}


