/***************************************************************************
This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10
Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)
This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/
/***************************************************************************
 File modified for VSC project
 -----------------------------------------------------------------------------*/

#include "VSCOB.h"
#include "VSCOBApplication.h"
#include "VSCOBScene.h"
#include "VSCOBBetaGUIListener.h"
#include "VSCOBCameraController.h"
#include "VSCOBInputAdapter.h"
#include "VSCOBKeyboardAction.h"

/*
 *  OgreBullet Shapes
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

/**
 *  
 */
#include "Utils/OgreBulletCollisionsMeshToShapeConverter.h"
#include "OgreBulletCollisionsRay.h"
#include "Debug/OgreBulletCollisionsDebugLines.h"

#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "OgreBulletDynamicsConstraint.h"
#include "Constraints/OgreBulletDynamicsPoint2pointConstraint.h" 

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

void VSC::OB::Scene::Element::destroy()
{
    Scene::SPtr scene = this->getScene().lock();
    scene->getDynamicsWorld()->removeObject(mRigidBody);
    
    delete mRigidBody;
    mRigidBody = 0;
}

void VSC::OB::Scene::ElementFactory::registerElement(Scene::Element::SPtr element)
{
    /*
     *  TODO: check that the element is not already registered
     */
    
    mSElements.push_back(element);
    mWElements.push_back(Scene::Element::WPtr(element));
}

void VSC::OB::Scene::ElementFactory::reset(void)
{
    /*
     *  Clearing the smart pointers in this container should deallocate the element,
     *  causing them to call their destroy() method.
     */

    mWElements.clear();
    mSElements.clear();
}


void VSC::OB::Scene::ElementFactory::destroyElement(Scene::Element::WPtr element)
{
    /*
     *  Remove the element from the scene
     */
    
    WElements::iterator wit = std::find(mWElements.begin(), mWElements.end(), element);
    if (wit != mWElements.end())
    {
        mWElements.erase(wit);
    }
    
    /*
     *  Erasing the smart pointer (which should be the only one), should result in the 
     *  element's destructor (and hence destroy() method) to be called, which should 
     *  remove all associated ogre representation.
     */
    
    SElements::iterator sit = std::find(mSElements.begin(), mSElements.end(), element.lock());
    if (sit != mSElements.end())
    {
        mSElements.erase(sit);
    }
    
}


VSC::OB::Scene::Element::WPtr VSC::OB::Scene::ElementFactory::elementWithRigidBody(OgreBulletDynamics::RigidBody* rigidBody)
{
    BOOST_FOREACH (Scene::Element::SPtr element, mSElements)
    {
        if (element->getRigidBody() == rigidBody)
        {
            return Scene::Element::WPtr(element);
        }
    }
    
    return Scene::Element::WPtr();
}


//MARK: Basic Constructor which does abolutely nothing interesting at all

VSC::OB::Scene::Scene() :
mCamera(0),
mRoot(0),
mSceneMgr(0),
mWindow(0),
mWorld(0),
mPaused(false),
mDebugRayLine(0)
{

}
// -------------------------------------------------------------------------
void VSC::OB::Scene::init(Ogre::Root *root, Ogre::RenderWindow *win)
{
    /*
     *  A bit of OGRE reading ...
     *  - Ogre::Root http://www.ogre3d.org/docs/api/html/classOgre_1_1Root.html
     *  - Ogre::RenderWindow http://www.ogre3d.org/docs/api/html/classOgre_1_1RenderWindow.html
     */
    
    mRoot = root;
    mWindow = win; 

    /******************* CREATESHADOWS If not debug mode ***************************/
#ifndef _DEBUG
    
    // Should we set the shadow technique once and for all? http://www.ogre3d.org/docs/manual/manual_70.html
    
    mCurrentShadowTechnique = SHADOWTYPE_TEXTURE_ADDITIVE;

    /**
     *  QUESTION: Where does mSceneMgr get set to something other than 0? ANSWER: In the subclasses (Demos) of course.
     *  NOTE: Ogre::SceneManager is an absolutely massive class http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneManager.html
     */
    
	mSceneMgr->setShadowColour(Ogre::ColourValue(0.5, 0.5, 0.5));

	Ogre::PixelFormat pxlFmt = Ogre::PF_L8;
	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(RSC_TEXTURE_FLOAT))
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
		mSceneMgr->setShadowTextureSelfShadow(true);
		mSceneMgr->setShadowTextureCasterMaterial(CUSTOM_CASTER_MATERIAL);
		mSceneMgr->setShadowTextureReceiverMaterial(CUSTOM_RECEIVER_MATERIAL + "/PCF");
		//mSceneMgr->setShadowTextureReceiverMaterial(CUSTOM_RECEIVER_MATERIAL);
	}

	/**
     *  Set the shadow camera setup to something which looks pretty complicated
     *  http://www.ogre3d.org/docs/api/html/classOgre_1_1LiSPSMShadowCameraSetup.html
     */
	
	Ogre::LiSPSMShadowCameraSetup *mLiSPSMSetup = new Ogre::LiSPSMShadowCameraSetup();
	mLiSPSMSetup->setUseAggressiveFocusRegion(true);
	//mLiSPSMSetup->setUseSimpleOptimalAdjust(true);
	mLiSPSMSetup->setOptimalAdjustFactor(1.1f);
	mSceneMgr->setShadowCameraSetup(Ogre::ShadowCameraSetupPtr(mLiSPSMSetup));

	mSceneMgr->setShadowTechnique(mCurrentShadowTechnique);
	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(RSC_HWRENDER_TO_TEXTURE))
	{
		// In D3D, use a 1024x1024 shadow texture
		mSceneMgr->setShadowTextureSettings(2048, 2, pxlFmt);
	}
	else
	{
		// Use 512x512 texture in GL since we can't go higher than the window res
		mSceneMgr->setShadowTextureSettings(512, 2, pxlFmt);
	}
#endif // _DEBUG
    
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

// -------------------------------------------------------------------------
void VSC::OB::Scene::setupLights()
{
    // Set ambient light
    mSceneMgr->setAmbientLight(ColourValue(0.4, 0.4, 0.4));
    
    Ogre::Light* l = 0;
    
	// Fixed light, dim
    l = mSceneMgr->createLight("Sunlight");
 	l->setPosition(0.0, 30.5, 0.0);
 	l->setCastShadows(false);
 	l->setType(Ogre::Light::LT_POINT);
 	l->setDiffuseColour(g_minLightColour);
 	l->setSpecularColour(0.2, 0.2, 0.2);
 	l->setAttenuation(8000, 1, 0.0005, 0);
    mLightMap.insert(LightMap::value_type("Sunlight", l));

	// Point light, movable, reddish
	l = mSceneMgr->createLight("Spot1");
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

	l = mSceneMgr->createLight("Spot2");
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

void VSC::OB::Scene::getDebugRayLines()
{
    if (mDebugRayLine == 0)
    {
        mDebugRayLine = new DebugLines();
        mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mDebugRayLine);
    }
}


void VSC::OB::Scene::shutdown ()
{
    /*
     *  It is important to reset the element factory before destroying the scene manager
     *  as it will use it when performing the reset.
     */
    
    mElementFactory->reset();

    delete mWorld->getDebugDrawer();
    mWorld->setDebugDrawer(0);
    delete mWorld;
    
    mSceneMgr->destroyCamera(mCamera->getName());
    mWindow->removeViewport(0);
    
    mRoot->destroySceneManager(mSceneMgr);
    
    delete mDebugRayLine;
    
    mWorld = 0;
    mCamera = 0;
    mSceneMgr = 0;
    mDebugRayLine = 0;

}


bool VSC::OB::Scene::resetCameraAspectRatio(void)
{
    Ogre::Viewport* vp = mCamera->getViewport();
    mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
    return true;
}

// -------------------------------------------------------------------------
VSC::OB::Scene::Element::WPtr VSC::OB::Scene::getElementAtViewportCoordinate(const Ogre::Viewport* v, Ogre::Vector2 p, Ogre::Vector3 &ip, Ogre::Ray &r)
{
    if (viewport == mCamera->getViewport())
    {
        rayTo = mCamera->getCameraToViewportRay(position.x, position.y);
        CollisionClosestRayResultCallback callback(rayTo, mWorld, mCamera->getFarClipDistance());
        mWorld->launchRay(callback);
        
        if (callback->doesCollide())
        {
            OgreBulletDynamics::RigidBody* body = static_cast <OgreBulletDynamics::RigidBody*>(callback->getCollidedObject());
            intersectionPoint = callback->getCollisionPoint();
            if (body)
            {
                return this->getElementFactory()->elementWithRigidBody(body);
            }
        }
    }
    
    return VSC::OB::Scene::Element::WPtr();
}

// -------------------------------------------------------------------------
bool VSC::OB::Scene::frameStarted(Real elapsedTime)
{
    
    if (mTraceFrame) std::cout << "VSC::OB::Scene::frameStarted, elapsed time " << elapsedTime << std::endl;
    
    this->getCameraController()->frameStarted(elapsedTime);

    // update physics
    if (!mPaused || mDoOnestep)
    {
        mWorld->stepSimulation(elapsedTime);
    }
    
    /*
     *  This is were the serious shit is going to happen, tracking collisions and such
     */

    mDoOnestep = false;

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

void drawText(bool draw)
{
    mDrawText = draw;
    mWorld->getDebugDrawer()->setDrawText(draw);
}

void drawFeaturesText(bool draw)
{
    mDrawFeaturesText = draw;
    mWorld->getDebugDrawer()->setDrawFeaturesText(draw);
}

void drawContactPoints(bool draw)
{
    mDrawContactPoints = draw;
    mWorld->getDebugDrawer()->setDrawContactPoints(draw);
    mWorld->setShowDebugContactPoints(draw);
}

void enableBulletLCP(bool enable)
{
    mDisableBulletLCP = !enable;
    mWorld->getDebugDrawer()->setDisableBulletLCP(mDisableBulletLCP);
}

void enableCCD(bool enable)
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
    
    this->updateStats();
    
    return true;
}

// ------------------------------------------------------------------------- 
bool VSC::OB::Scene::checkIfEnoughPlaceToAddObject(float minDist)
{
    Ogre::Vector3 pickPos;
    Ogre::Ray rayTo;
    OgreBulletDynamics::RigidBody * body = getBodyUnderCursorUsingBullet(pickPos, rayTo);

    if (body)
    {          
        if ((pickPos - mCamera->getDerivedPosition ()).length () < minDist)
            return false;
    }
    
    return true;        
}


// -------------------------------------------------------------------------
void VSC::OB::Scene::initWorld(const Ogre::Vector3 &gravityVector, const Ogre::AxisAlignedBox &bounds)
{
    // Start Bullet
    mWorld = new OgreBulletDynamics::DynamicsWorld(mSceneMgr, bounds, gravityVector, true, true, 10000);

    // add Debug info display tool
    OgreBulletCollisions::DebugDrawer *debugDrawer = new OgreBulletCollisions::DebugDrawer();

    mWorld->setDebugDrawer(debugDrawer);

    Ogre::SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("DebugDrawer", Ogre::Vector3::ZERO);
    node->attachObject (static_cast<SimpleRenderable*>(debugDrawer));
}

// -------------------------------------------------------------------------
void VSC::OB::Scene::addGround()
{
    this->addStaticPlane(gStaticBodyRestitution, gStaticBodyFriction);
}

// -------------------------------------------------------------------------
const StatsMap& VSC::OB::Scene::getUpdatedStatsMap(void)
{
    const Ogre::RenderTarget::FrameStats& stats = mWindow->getStatistics();
    
    mStatsMap["Average FPS"]    = Ogre::StringConverter::toString(stats.avgFPS);
    mStatsMap["Current FPS"]    = Ogre::StringConverter::toString(stats.lastFPS);
    mStatsMap["Triangle Count"] = Ogre::StringConverter::toString(stats.triangleCount);
    
    return mStatsMap;
}

