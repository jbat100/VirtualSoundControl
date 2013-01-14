/*
 *  VSC Stuff
 */

#include "VSCOB.h"
#include "VSCException.h"
#include "VSCOBApplication.h"
#include "VSCOBScene.h"
#include "VSCOBElementFactory.h"

#include "OgreBulletCollisions.h"
#include "OgreBulletDynamicsWorld.h"
#include "Debug/OgreBulletCollisionsDebugLines.h"

#include "btBulletCollisionCommon.h"

#include <boost/foreach.hpp>

#include <limits>

using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

/*
 *  GLOBAL VARIABLES
 */

// -------------------------------------------------------------------------
const Ogre::ColourValue g_minLightColour(0.2, 0.1, 0.0);
const Ogre::ColourValue g_maxLightColour(0.5, 0.3, 0.1);
// -------------------------------------------------------------------------

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
	//sceneManager->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(mLiSPSMSetup));

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
    mElementFactory = ElementFactory::SPtr((ElementFactory*) new ElementFactory(shared_from_this()));
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
    
    BOOST_FOREACH(VSC::Listener::WPtr listener, this->getListeners())
    {
        Scene::Listener::SPtr sceneListener = boost::dynamic_pointer_cast<Scene::Listener>(listener.lock());
        if (sceneListener)
        {
            sceneListener->sceneRegisteredElement(shared_from_this(), element);
        }
    }
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
    
    if ((!this->getSetting(SettingPaused)) || mDoOneStep)
    {
        mDynamicsWorld->stepSimulation(elapsedTime);
    }
    
    if (this->getCollisionDetector())
    {
        this->getCollisionDetector()->updateCollisions();
    }

    mDoOneStep = false;
    
    BOOST_FOREACH(VSC::Listener::WPtr listener, this->getListeners())
    {
        Scene::Listener::SPtr sceneListener = boost::dynamic_pointer_cast<Scene::Listener>(listener.lock());
        if (sceneListener)
        {
            sceneListener->sceneWasRendered(shared_from_this());
        }
    }

    return true;
}

void VSC::OB::Scene::toggleSetting(Setting setting)
{
    this->setSetting(setting, !this->getSetting(setting));
}

void VSC::OB::Scene::setSetting(Setting setting, bool on)
{
    mSettingValueMap[setting] = on;
    
    switch (setting)
    {
        case SettingDrawWireFrame:
            mDynamicsWorld->getDebugDrawer()->setDrawWireframe(on);
            mDynamicsWorld->setShowDebugShapes(on);
            break;
            
        case SettingDrawAABB:
            mDynamicsWorld->getDebugDrawer()->setDrawAabb(on);
            break;
            
        case SettingDrawText:
            mDynamicsWorld->getDebugDrawer()->setDrawText(on);
            break;
            
        case SettingDrawFeaturesText:
            mDynamicsWorld->getDebugDrawer()->setDrawFeaturesText(on);
            break;
            
        case SettingDrawContactPoints:
            mDynamicsWorld->getDebugDrawer()->setDrawContactPoints(on);
            mDynamicsWorld->setShowDebugContactPoints(on);
            break;
            
        case SettingBulletLCPIEnabled:
            mDynamicsWorld->getDebugDrawer()->setDisableBulletLCP(on);
            break;
            
        case SettingCCDEnabled:
            mDynamicsWorld->getDebugDrawer()->setEnableCCD(on);
            break;
            
        case SettingSatComparisonEnabled:
            mDynamicsWorld->getDebugDrawer()->setEnableSatComparison(on);
            break;
            
        case SettingNoDeactivation:
            mDynamicsWorld->getDebugDrawer()->setNoDeactivation(on);
            break;
            
        case SettingProfileTimings:
            mDynamicsWorld->getDebugDrawer()->setProfileTimings(on);
            break;
            
        default:
            break;
    }
    
    BOOST_FOREACH(VSC::Listener::WPtr listener, this->getListeners())
    {
        Scene::Listener::SPtr sceneListener = boost::dynamic_pointer_cast<Scene::Listener>(listener.lock());
        if (sceneListener)
        {
            sceneListener->sceneChangedSetting(shared_from_this(), setting, on);
        }
    }
}

bool VSC::OB::Scene::getSetting(Setting setting)
{
    return mSettingValueMap[setting];
}


// -------------------------------------------------------------------------
bool VSC::OB::Scene::frameEnded(const Ogre::FrameEvent& evt)
{
    Real elapsedTime = evt.timeSinceLastFrame;
    
    if (mTraceFrame) std::cout << "VSC::OB::Scene::frameEnded, elapsed time " << elapsedTime << std::endl;
    
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


