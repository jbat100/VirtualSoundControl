/*
 *  VSC Stuff
 */

#include "VSCOB.h"
#include "VSCOBApplication.h"
#include "VSCOBScene.h"
#include "VSCOBBetaGUIListener.h"
#include "VSCOBCameraController.h"
#include "VSCOBInputAdapter.h"
#include "VSCOBKeyboardAction.h"
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

/*
 *  Bullet Stuff
 */
#include "btBulletCollisionCommon.h"

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

//MARK: Element Factory

/*
void VSC::OB::Scene::ElementFactory::registerElement(Scene::Element::SPtr element)
{
    
    mSElements.push_back(element);
}
 */



//MARK: - Collision

/*
 
void VSC::OB::Collision::addPersistentManifold(btPersistentManifold* manifold)
{
    PersistentManifolds::iterator it = std::find(mPersistentManifolds.begin(), mPersistentManifolds.end(), manifold);
    
    if (it != mPersistentManifolds.end()) {
        mPersistentManifolds.push_back(manifold);
    }
}

void VSC::OB::Collision::clearPersistentManifolds()
{
    mPersistentManifolds.clear();
}
 */

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


void VSC::OB::Scene::CollisionDetector::addListener(CollisionListener::SPtr listener)
{
    Listeners::iterator it = std::find(mListeners.begin(), mListeners.end(), listener);
    if (it == mListeners.end()) {
        mListeners.push_back(listener);
    }
}

void VSC::OB::Scene::CollisionDetector::removeListener(CollisionListener::SPtr listener)
{
    Listeners::iterator it = std::find(mListeners.begin(), mListeners.end(), listener);
    if (it != mListeners.end()) {
        mListeners.erase(it);
    }
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
    
    Scene::SPtr scene = this->getScene().lock();
    
    /*
     *  Clear current persistent manifolds to repopulate them with the current world ones.
     *  There can be several manifolds for each object pair which is kind of annoying...
     */
    
    /*
    BOOST_FOREACH (Collision::SPtr collision, mCollisions)
    {
        if (collision)
        {
            collision->clearPersistentManifolds();
        }
    }
     */
    
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
                BOOST_FOREACH (CollisionListener::SPtr listener, mListeners) if (listener) listener->collisionProspectEnded(collision);
            }
            
            else if (collision->getState() == Collision::StateOngoing)
            {
                collision->setState(Collision::StateEnded);
                BOOST_FOREACH (CollisionListener::SPtr listener, mListeners) if (listener) listener->collisionEnded(collision);
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
                BOOST_FOREACH (CollisionListener::SPtr listener, mListeners) if (listener) listener->collisionDetected(collision);
            }
            else if (collision->getState() == Collision::StateOngoing)
            {
                BOOST_FOREACH (CollisionListener::SPtr listener, mListeners) if (listener) listener->collisionUpdated(collision);
            }
        }
        
        else if (minDistance > mDistanceThreshold)
        {
            if (collision->getState() == Collision::StateNone      ||
                collision->getState() == Collision::StateCreated   ||
                collision->getState() == Collision::StateEnded)
            {
                collision->setState(Collision::StateClose);
                BOOST_FOREACH (CollisionListener::SPtr listener, mListeners) if (listener) listener->collisionProspectDetected(collision);
            }
            else if (collision->getState() == Collision::StateOngoing)
            {
                collision->setState(Collision::StateEnded);
                BOOST_FOREACH (CollisionListener::SPtr listener, mListeners) if (listener) listener->collisionEnded(collision);
                collision->setState(Collision::StateClose);
                BOOST_FOREACH (CollisionListener::SPtr listener, mListeners) if (listener) listener->collisionProspectDetected(collision);
            }
            else if (collision->getState() == Collision::StateClose)
            {
                BOOST_FOREACH (CollisionListener::SPtr listener, mListeners) if (listener) listener->collisionProspectUpdated(collision);
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


//MARK: - Basic Constructor which does abolutely nothing interesting at all

VSC::OB::Scene::Scene() :
mCamera(0),
mRoot(0),
mSceneManager(0),
mWindow(0),
mWorld(0),
mPaused(false),
mDebugRayLine(0)
{

}

// -------------------------------------------------------------------------

void VSC::OB::Scene::init(Application_SPtr application)
{
    BOOST_ASSERT(application);
    if(!application) return;
    
    mApplication = Application_WPtr(application);
    
    /*
     *  A bit of OGRE reading ...
     *  - Ogre::Root http://www.ogre3d.org/docs/api/html/classOgre_1_1Root.html
     *  - Ogre::RenderWindow http://www.ogre3d.org/docs/api/html/classOgre_1_1RenderWindow.html
     */
    
    Ogre::Root* root = application->getRoot();
    Ogre::SceneManager* sceneManager = this->getSceneManager();
    
    BOOST_ASSERT(root);
    BOOST_ASSERT(sceneManager);
    if (!(root && sceneManage)) return;

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
    
    Scene::WPtr weakThis = Scene::WPtr(shared_from_this());
    this->setCollisionDetector(CollisionDetector::SPtr(new CollisionDetector(weakThis)));
    
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

}

void VSC::OB::Scene::setupFactory()
{
    mElementFactory = ElementFactory::SPtr((ElementFactory*) new BasicSceneElementFactory(shared_from_this()));
}

void VSC::OB::Scene::setupLights()
{
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

OgreBulletCollisions::DebugLines* VSC::OB::Scene::getDebugRayLines()
{
    if (mDebugRayLine == 0)
    {
        mDebugRayLine = new OgreBulletCollisions::DebugLines();
        sceneManager->getRootSceneNode()->createChildSceneNode()->attachObject(mDebugRayLine);
    }
    
    return mDebugRayLine;
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



void VSC::OB::Scene::shutdown ()
{
    /*
     *  It is important to reset the element factory before destroying the scene manager
     *  as it will use it when performing the reset.
     */
    
    this->destroyAllElements();
    
    if (mWorld) {
        if (mWorld->getDebugDrawer()) {
            delete mWorld->getDebugDrawer();
            mWorld->setDebugDrawer(0);
        }
        mWorld->setDebugDrawer(0);
        delete mWorld;
        mWorld = 0;
    }
    
    if (sceneManager) {
        if (mCamera) {
            sceneManager->destroyCamera(mCamera->getName());
        }
        mCamera = 0;
        if (mRoot) {
            mRoot->destroySceneManager(sceneManager);
        }
        sceneManager = 0;
    }
    
    if (mWindow) {
        mWindow->removeViewport(0);
    }
    
    if (mDebugRayLine) {
        delete mDebugRayLine;
        mDebugRayLine = 0;
    }

}


bool VSC::OB::Scene::resetCameraAspectRatio(void)
{
    Ogre::Viewport* vp = mCamera->getViewport();
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
    return true;
}

// -------------------------------------------------------------------------

VSC::OB::Scene::Element::SPtr VSC::OB::Scene::getElementAtViewportCoordinate(const Ogre::Viewport* v,
                                                                             Ogre::Vector2& p,
                                                                             Ogre::Vector3 &ip,
                                                                             Ogre::Ray &r)
{
    if (v == mCamera->getViewport())
    {
        r = mCamera->getCameraToViewportRay(p.x, p.y);
        CollisionClosestRayResultCallback callback(r, mWorld, mCamera->getFarClipDistance());
        mWorld->launchRay(callback);
        
        if (callback.doesCollide())
        {
            OgreBulletDynamics::RigidBody* body = static_cast <OgreBulletDynamics::RigidBody*>(callback.getCollidedObject());
            ip = callback.getCollisionPoint();
            if (body)
            {
                return this->getElementWithRigidBody(body);
            }
        }
    }
    
    return Element::SPtr();
}

// -------------------------------------------------------------------------
bool VSC::OB::Scene::frameStarted(Real elapsedTime)
{
    
    if (mTraceFrame) std::cout << "VSC::OB::Scene::frameStarted, elapsed time " << elapsedTime << std::endl;
    
    //this->getCameraController()->frameStarted(elapsedTime);

    // update physics
    if (!mPaused || mDoOneStep)
    {
        mWorld->stepSimulation(elapsedTime);
    }
    
    /*
     *  This is were the serious shit is going to happen, tracking collisions and such
     */
    
    if (this->getCollisionDetector()) {
        this->getCollisionDetector()->updateCollisions();
    }

    mDoOneStep = false;

    return true;
}

void VSC::OB::Scene::drawWireFrame(bool draw)
{
    mDrawWireFrame = draw;
    mWorld->getDebugDrawer()->setDrawWireframe(draw);
    mWorld->setShowDebugShapes(draw);
}

void VSC::OB::Scene::drawAabb(bool draw)
{
    mDrawAabb = draw;
    mWorld->getDebugDrawer()->setDrawAabb(draw);
}

void VSC::OB::Scene::drawText(bool draw)
{
    mDrawText = draw;
    mWorld->getDebugDrawer()->setDrawText(draw);
}

void VSC::OB::Scene::drawFeaturesText(bool draw)
{
    mDrawFeaturesText = draw;
    mWorld->getDebugDrawer()->setDrawFeaturesText(draw);
}

void VSC::OB::Scene::drawContactPoints(bool draw)
{
    mDrawContactPoints = draw;
    mWorld->getDebugDrawer()->setDrawContactPoints(draw);
    mWorld->setShowDebugContactPoints(draw);
}

void VSC::OB::Scene::enableBulletLCP(bool enable)
{
    mDisableBulletLCP = !enable;
    mWorld->getDebugDrawer()->setDisableBulletLCP(mDisableBulletLCP);
}

void VSC::OB::Scene::enableCCD(bool enable)
{
    mEnableCCD = enable;
    mWorld->getDebugDrawer()->setEnableCCD(mEnableCCD);
}

// -------------------------------------------------------------------------
bool VSC::OB::Scene::frameEnded(Real elapsedTime)
{
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

// ------------------------------------------------------------------------- 
bool VSC::OB::Scene::checkIfEnoughPlaceToAddObject(float minDist)
{
    Ogre::Vector3 pickPos;
    Ogre::Ray rayTo;
    
    Ogre::Vector2 coord(0.5, 0.5);
    Scene::Element::SPtr element = this->getElementAtViewportCoordinate(mCamera->getViewport(), coord, pickPos, rayTo);
    
    if (element)
    {
        OgreBulletDynamics::RigidBody *body = element->getRigidBody();
        
        if (body)
        {
            if ((pickPos - mCamera->getDerivedPosition ()).length () < minDist)
                return false;
        }
    }
    
    return true;        
}


// -------------------------------------------------------------------------
void VSC::OB::Scene::initWorld(const Ogre::Vector3 &gravityVector, const Ogre::AxisAlignedBox &bounds)
{
    // Start Bullet
    mWorld = new OgreBulletDynamics::DynamicsWorld(sceneManager, bounds, gravityVector, true, true, 10000);

    // add Debug info display tool
    OgreBulletCollisions::DebugDrawer *debugDrawer = new OgreBulletCollisions::DebugDrawer();

    mWorld->setDebugDrawer(debugDrawer);

    Ogre::SceneNode *node = sceneManager->getRootSceneNode()->createChildSceneNode("DebugDrawer", Ogre::Vector3::ZERO);
    node->attachObject (static_cast<SimpleRenderable*>(debugDrawer));
}

// -------------------------------------------------------------------------
const VSC::OB::Scene::StatsMap& VSC::OB::Scene::getUpdatedStatsMap(void)
{
    const Ogre::RenderTarget::FrameStats& stats = mWindow->getStatistics();
    
    mStatsMap["Average FPS"]    = Ogre::StringConverter::toString(stats.avgFPS);
    mStatsMap["Current FPS"]    = Ogre::StringConverter::toString(stats.lastFPS);
    mStatsMap["Triangle Count"] = Ogre::StringConverter::toString(stats.triangleCount);
    
    return mStatsMap;
}

