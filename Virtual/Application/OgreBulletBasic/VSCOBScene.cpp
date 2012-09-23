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

size_t VSC::OB::Scene::mNumEntitiesInstanced = 0;

// -------------------------------------------------------------------------
const Ogre::ColourValue g_minLightColour(0.2, 0.1, 0.0);
const Ogre::ColourValue g_maxLightColour(0.5, 0.3, 0.1);
// -------------------------------------------------------------------------

const float             gStaticBodyRestitution  = 0.1f;
const float             gStaticBodyFriction     = 0.8f;

const float             gDynamicBodyRestitution = 0.6f;
const float             gDynamicBodyFriction    = 0.6f;
const float             gDynamicBodyMass        = 1.0f;

const Ogre::Vector3     gCubeBodyBounds      = Ogre::Vector3 (1, 1, 1);
const Ogre::Vector3     gCylinderBodyBounds  = Ogre::Vector3 (0.5, 1, 0.5); 
const Ogre::Vector3     gConeBodyBounds      = Ogre::Vector3 (1, 1, 1);
const Real              gSphereBodyBounds    = 1.0f;
// -------------------------------------------------------------------------


//MARK: Basic Constructor which does abolutely nothing interesting at all 

VSC::OB::Scene::Scene() :    
mCamera(0),
mRoot(0),
mSceneMgr(0),
mWindow(0),
mWorld(0),
mPaused (false),
mDebugRayLine(0),
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
    
    mCurrentShadowTechnique = SHADOWTYPE_TEXTURE_ADDITIVE;

    /**
     *  QUESTION: Where does mSceneMgr get set to something other than 0? ANSWER: In the subclasses (Demos) of course.
     *  NOTE: Ogre::SceneManager is an absolutely massive class http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneManager.html
     */
    
	mSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));

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

    /******************* CREATE Queries ***************************/
    
    mRayQuery = mSceneMgr->createRayQuery(Ray());
    mRayQuery->setQueryMask(VSC::OB::QueryMaskGeometry);
    mRayQuery->setQueryTypeMask(SceneManager::ENTITY_TYPE_MASK);
    Ogre::MovableObject::setDefaultQueryFlags(VSC::OB::QueryMaskAny);

    mPickConstraint = 0;
    
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
    l = mSceneMgr->createLight("Sun");
 	l->setPosition(0.0, 30.5, 0.0);
 	l->setCastShadows(false);
 	l->setType(Ogre::Light::LT_POINT);
 	l->setDiffuseColour(g_minLightColour);
 	l->setSpecularColour(0.2, 0.2, 0.2);
 	l->setAttenuation(8000, 1, 0.0005, 0);
    mLightMap.insert(LightMap::value_type("Sunlight", l))
    

	// Point light, movable, reddish
	mLight = mSceneMgr->createLight("Spot");
	mLight->setType(Light::LT_SPOTLIGHT);
	mLight->setPosition(100.0, 80.5,-10.0);
	mLight->setSpotlightRange(Degree(30), Degree(50));
	Ogre::Vector3 dir;
	dir = -mLight->getPosition();
	dir.normalise();
	mLight->setDirection(dir);
	mLight->setDiffuseColour(0.70, 0.70, 0.72);
	mLight->setSpecularColour(0.1, 0.1, 0.3);


	mLight2 = mSceneMgr->createLight("Spot2");
	mLight2->setType(Light::LT_SPOTLIGHT);
	mLight2->setPosition(-100.0, 80.5, 10.0);
	mLight2->setSpotlightRange(Degree(30), Degree(50));
	dir = -mLight2->getPosition();
	dir.normalise();
	mLight2->setDirection(dir);
	mLight2->setDiffuseColour(0.72, 0.70, 0.70);
	mLight2->setSpecularColour(0.3, 0.1, 0.1);

}

void VSC::OB::Scene::getDebugLines()
{
    if (mDebugRayLine == 0)
    {
        mDebugRayLine = new DebugLines();
        mSceneMgr->getRootSceneNode()->createChildSceneNode()->attachObject(mDebugRayLine);
    }
}


void VSC::OB::Scene::shutdown ()
{


    // OgreBullet physic delete 
    std::deque<OgreBulletDynamics::RigidBody *>::iterator itBody = mBodies.begin();
    while (mBodies.end() != itBody)
    {   
        delete *itBody; 
        ++itBody;
    }

    delete mWorld->getDebugDrawer();
    mWorld->setDebugDrawer(0);
    delete mWorld;

    // Ogre delete 
    mSceneMgr->destroyQuery (mRayQuery);
    std::deque<Ogre::Entity *>::iterator itEntity = mEntities.begin();
    while (mEntities.end() != itEntity)
    {
        SceneNode *node = static_cast<SceneNode*>((*itEntity)->getParentNode());
        node->detachObject(*itEntity);
        mSceneMgr->destroyEntity(*itEntity);
        assert (node->getParent());
        static_cast <SceneNode *> (node->getParent())->removeAndDestroyChild(node->getName());
        ++itEntity;
    }
    
    mEntities.clear();
    
    mSceneMgr->destroyCamera(mCamera->getName());
    mWindow->removeViewport(0);
    mRoot->destroySceneManager(mSceneMgr);
    
    delete mDebugRayLine;
    
    mRayQuery = 0;
    mWorld = 0;
    mCamera = 0;
    mSceneMgr = 0;
    mDebugRayLine = 0;

    mBodies.clear();
    mEntities.clear();
    mShapes.clear();

}


bool VSC::OB::Scene::renderWindowChangedSize(Ogre::RenderWindow* renderWindow)
{
    if (renderWindow == mWindow)
    {
        Ogre::Viewport* vp = mCamera->getViewport();
        mCamera->setAspectRatio(Ogre::Real(vp->getActualWidth()) / Ogre::Real(vp->getActualHeight()));
        return true;
    }
    
    return false;
}

// -------------------------------------------------------------------------
OgreBulletDynamics::RigidBody* VSC::OB::Scene::getBodyUnderCursorUsingBullet(Ogre::Vector3 &intersectionPoint, Ray &rayTo)
{
    
    float absX = this->getInputAdapter()->getLastMousePosition().x;
    float absY = this->getInputAdapter()->getLastMousePosition().y;
    
    Ogre::Viewport* viewport = mCamera->getViewport();
    
    float normX = absX / (float) viewport->getActualWidth();
    float normY = 1.0 - (absY / (float) viewport->getActualHeight());
    
    /*
     *  See http://www.ogre3d.org/docs/api/html/classOgre_1_1Viewport.html
     *  It seems to me that these x/y coordinates should be 0-1 (normalised)
     */
    
    rayTo = mCamera->getCameraToViewportRay (normX, normY);

	delete mCollisionClosestRayResultCallback;
	mCollisionClosestRayResultCallback = new CollisionClosestRayResultCallback(rayTo, mWorld, mCamera->getFarClipDistance());

    mWorld->launchRay(*mCollisionClosestRayResultCallback);
    
    if (mCollisionClosestRayResultCallback->doesCollide ())
    {
        OgreBulletDynamics::RigidBody * body = static_cast <OgreBulletDynamics::RigidBody*>
            (mCollisionClosestRayResultCallback->getCollidedObject());
		
		intersectionPoint = mCollisionClosestRayResultCallback->getCollisionPoint ();
        setDebugText("Hit :" + body->getName());
        return body;
    }
    
    return 0;
}
// -------------------------------------------------------------------------
OgreBulletDynamics::RigidBody* VSC::OB::Scene::getBodyUnderCursorUsingOgre(Ogre::Vector3 &intersectionPoint, Ray &rayTo)
{
    float absX = this->getInputAdapter()->getLastMousePosition().x;
    float absY = this->getInputAdapter()->getLastMousePosition().y;
    
    Ogre::Viewport* viewport = mCamera->getViewport();
    
    float normX = absX / (float) viewport->getActualWidth();
    float normY = 1.0 - (absY / (float) viewport->getActualHeight());
    
    //if (mTraceUI) std::cout << "VSC::OB::Scene::getBodyUnderCursorUsingOgre norm coord: (" << normX << "," << normY << ")" << std::endl;
    //if (mTraceUI) std::cout << "VSC::OB::Scene::getBodyUnderCursorUsingOgre viewport is " << viewport << std::endl;
    
    //rayTo = mCamera->getCameraToViewportRay (absX, absY);
    rayTo = mCamera->getCameraToViewportRay (normX, normY);
    
    if (mTraceUI) std::cout << "VSC::OB::Scene::getBodyUnderCursorUsingOgre rayTo is " << &rayTo << std::endl;

    mRayQuery->setRay (rayTo);
    
    const RaySceneQueryResult& result = mRayQuery->execute();
    
    if (!result.empty())
    {
        if (mTraceUI) std::cout << "VSC::OB::Scene::getBodyUnderCursorUsingOgre results has " << result.size() << " elements" << std::endl;
        
        RaySceneQueryResult::const_iterator i = result.begin();

        mRayQuery->setSortByDistance (true, 1);//only one hit
        
        while((i != result.end()))
        {
            SceneNode *node = i->movable->getParentSceneNode() ;
			intersectionPoint = node->_getDerivedPosition ();
            const unsigned short num = node->numAttachedObjects();
            
            MovableObject* movable;
            
            for (unsigned short cur = 0; cur < num; cur++)
            {
                movable = node->getAttachedObject(cur);
                
                if (mTraceUI) std::cout << "VSC::OB::Scene::getBodyUnderCursorUsingOgre movable->getMovableType() " << movable->getMovableType() << std::endl;
                
                /*
                 *  Does not work, use ray casts using bullet, for some reason to be investigated we are getting back a simple Ogre::MovableObject 
                 *  (dynamic down cast does not work)
                 */
                
                OgreBulletCollisions::Object *object = dynamic_cast <OgreBulletCollisions::Object*>(movable);
                OgreBulletDynamics::RigidBody *body = dynamic_cast <OgreBulletDynamics::RigidBody*>(object);
                
                if (movable->getMovableType() == OgreBulletCollisions::Object::mMovableType)
                {
                    if (mTraceUI) std::cout << "VSC::OB::Scene::getBodyUnderCursorUsingOgre object " << movable << " is movable" << std::endl;
                    setDebugText ("Hit :" + body->getName());
                    return body;
                }
                else
                {
                    if (mTraceUI) std::cout << "VSC::OB::Scene::getBodyUnderCursorUsingOgre object " << movable << " is not movable" << std::endl;
                }
            }
            ++i;
        }	
    }
    
    else
    {
        if (mTraceUI) std::cout << "VSC::OB::Scene::getBodyUnderCursorUsingOgre results is empty" << std::endl;
    }
    
    return 0;
}
// -------------------------------------------------------------------------
bool VSC::OB::Scene::frameStarted(Real elapsedTime)
{
    
    if (mTraceFrame) std::cout << "VSC::OB::Scene::frameStarted, elapsed time " << elapsedTime << std::endl;
    
    if (mQuit)
        return false;
    
    this->getCameraController()->frameStarted(elapsedTime);

    // update physics
    if (!mPaused || mDoOnestep)
        mWorld->stepSimulation(elapsedTime);

    mDoOnestep = false;

    return true;
}
// -------------------------------------------------------------------------
bool VSC::OB::Scene::frameEnded(Real elapsedTime)
{
    if (mTraceFrame) std::cout << "VSC::OB::Scene::frameEnded, elapsed time " << elapsedTime << std::endl;
    
    if (mQuit)
        return false;

    DebugDrawer *debugDrawer = mWorld->getDebugDrawer();

    // Scene Debug Options
    if (mDrawWireFrame)
    {
        const bool wasWireframeShapes = debugDrawer->doesDrawWireframe();
        debugDrawer->setDrawWireframe(!wasWireframeShapes);
        mWorld->setShowDebugShapes(!wasWireframeShapes);
        mDrawWireFrame = false;
    }
    if (mDrawAabb) 
    {
        debugDrawer->setDrawAabb(!debugDrawer->doesDrawAabb());
        mDrawAabb = false;
    }
    if ( mDrawFeaturesText)
    {
        debugDrawer->setDrawFeaturesText(!debugDrawer->doesDrawFeaturesText());
        mDrawFeaturesText = false;
    }
    if ( mDrawContactPoints)
    {
        debugDrawer->setDrawContactPoints(!debugDrawer->doesDrawContactPoints());
		mWorld->setShowDebugContactPoints(debugDrawer->doesDrawContactPoints());
        mDrawContactPoints = false;
    }
    if ( mNoDeactivation)
    {
        debugDrawer->setNoDeactivation(!debugDrawer->doesNoDeactivation());
        mNoDeactivation = false;
    }
    if ( mNoHelpText)
    {
        debugDrawer->setNoHelpText(!debugDrawer->doesNoHelpText());
        mNoHelpText = false;
    }
    if ( mDrawText)
    {
        debugDrawer->setDrawText(!debugDrawer->doesDrawText());
        mDrawText = false;
    }
    if ( mProfileTimings)
    {
        debugDrawer->setProfileTimings(!debugDrawer->doesProfileTimings());
        mProfileTimings = false;
    }
    if ( mEnableSatComparison)
    {
        debugDrawer->setEnableSatComparison(!debugDrawer->doesEnableSatComparison());
        mEnableSatComparison = false;
    }
    if ( mDisableBulletLCP)
    {
        debugDrawer->setDisableBulletLCP(!debugDrawer->doesDisableBulletLCP());
        mDisableBulletLCP = false;
    }
    if ( mEnableCCD)
    {
        debugDrawer->setEnableCCD(!debugDrawer->doesEnableCCD());
        mEnableCCD = false;
    }

    mGuiListener->getGui()->update (elapsedTime);
    
    updateStats();
    
    return true;
}
// ------------------------------------------------------------------------- 
bool VSC::OB::Scene::checkIfEnoughPlaceToAddObject(float maxDist)
{
    Ogre::Vector3 pickPos;
    Ogre::Ray rayTo;
    OgreBulletDynamics::RigidBody * body = getBodyUnderCursorUsingBullet(pickPos, rayTo);
        //getBodyUnderCursorUsingOgre(pickPos, rayTo);
    if (body)
    {          
        if ((pickPos - mCamera->getDerivedPosition ()).length () < maxDist)
            return false;
    }
    return true;        
}
// -------------------------------------------------------------------------
bool VSC::OB::Scene::throwDynamicObject(VSC::OB::KeyboardAction::Key key)
{
    const float trowDist = 2.0f;
    
    bool handled = true;
    
    switch(key)
    {
        case VSC::OB::KeyboardAction::ShootCube: 
            
            if (this->checkIfEnoughPlaceToAddObject(trowDist))
            {
                const Ogre::Vector3 vec (mCamera->getDerivedPosition());
                OgreBulletDynamics::RigidBody *body = addCube("cube", vec, Quaternion(0,0,0,1), 
                                                              gCubeBodyBounds, gDynamicBodyRestitution, gDynamicBodyFriction, gDynamicBodyMass);
                
                body->setLinearVelocity(mCamera->getDerivedDirection().normalisedCopy() * mShootSpeed);
            }
            break;
            
        case VSC::OB::KeyboardAction::ShootSphere: 
            
            if (this->checkIfEnoughPlaceToAddObject(trowDist))
            {
                const Ogre::Vector3 vec (mCamera->getDerivedPosition());
                OgreBulletDynamics::RigidBody *body = addSphere("sphere", vec, Quaternion(0,0,0,1), 
                                                                gSphereBodyBounds, 
                                                                gDynamicBodyRestitution, gDynamicBodyFriction, gDynamicBodyMass);
                
                body->setLinearVelocity(mCamera->getDerivedDirection().normalisedCopy() * mShootSpeed);
            }
            break;
            
        case VSC::OB::KeyboardAction::ShootCylinder: 
            
            if (this->checkIfEnoughPlaceToAddObject(trowDist))
            {
                const Ogre::Vector3 vec (mCamera->getDerivedPosition());
                OgreBulletDynamics::RigidBody *body = addCylinder("cylinder", vec, Quaternion(0,0,0,1), 
                                                                  gCylinderBodyBounds, 
                                                                  gDynamicBodyRestitution, gDynamicBodyFriction, gDynamicBodyMass);
                
                body->setLinearVelocity(mCamera->getDerivedDirection().normalisedCopy() * mShootSpeed);
            }
            break;
            
        case VSC::OB::KeyboardAction::ShootCone: 
            
            if (this->checkIfEnoughPlaceToAddObject(trowDist))
            {
                const Ogre::Vector3 vec (mCamera->getDerivedPosition());
                OgreBulletDynamics::RigidBody *body = addCone("cone", vec, Quaternion(0,0,0,1), 
                                                              gConeBodyBounds, 
                                                              gDynamicBodyRestitution, gDynamicBodyFriction, gDynamicBodyMass);
                
                body->setLinearVelocity(mCamera->getDerivedDirection().normalisedCopy() * mShootSpeed);
            }
            break;
            
        default:
            handled = false;
            break;
    }
    
    return handled;
}

// -------------------------------------------------------------------------
bool VSC::OB::Scene::dropDynamicObject(VSC::OB::KeyboardAction::Key key)
{
    const float dropDist = 10.0f;
    
    bool handled = true;
    
    switch(key)
    {
        case VSC::OB::KeyboardAction::DropCube: 
            
            if (this->checkIfEnoughPlaceToAddObject(dropDist))
            {
                const Ogre::Vector3 vec (mCamera->getDerivedPosition());
                OgreBulletDynamics::RigidBody *body = addCube("cube", 
                                                              vec + mCamera->getDerivedDirection().normalisedCopy() * 10, 
                                                              Quaternion(0,0,0,1), 
                                                              gCubeBodyBounds, gDynamicBodyRestitution, gDynamicBodyFriction, gDynamicBodyMass);
                
            }
            break;
            
        case VSC::OB::KeyboardAction::DropSphere: 
            
            if (this->checkIfEnoughPlaceToAddObject(dropDist))
            {
                const Ogre::Vector3 vec (mCamera->getDerivedPosition());
                OgreBulletDynamics::RigidBody *body = addSphere("sphere", 
                                                                vec + mCamera->getDerivedDirection().normalisedCopy() * 10, 
                                                                Quaternion(0,0,0,1), 
                                                                gSphereBodyBounds, 
                                                                gDynamicBodyRestitution, gDynamicBodyFriction, gDynamicBodyMass);
                
            }
            break;
            
        case VSC::OB::KeyboardAction::DropCylinder : 
            
            if (this->checkIfEnoughPlaceToAddObject(dropDist))
            {
                const Ogre::Vector3 vec (mCamera->getDerivedPosition());
                OgreBulletDynamics::RigidBody *body = addCylinder("Cylinder", vec, Quaternion(0,0,0,1), 
                                                                  gCylinderBodyBounds, 
                                                                  gDynamicBodyRestitution, gDynamicBodyFriction, gDynamicBodyMass);
                
            }
            break;
            
        case VSC::OB::KeyboardAction::DropCone: 
            
            if (this->checkIfEnoughPlaceToAddObject(dropDist))
            {
                const Ogre::Vector3 vec (mCamera->getDerivedPosition());
                OgreBulletDynamics::RigidBody *body = addCone("Cone", 
                                                              vec + mCamera->getDerivedDirection().normalisedCopy() * 10, 
                                                              Quaternion(0,0,0,1), 
                                                              gConeBodyBounds, 
                                                              gDynamicBodyRestitution, gDynamicBodyFriction, gDynamicBodyMass);
            }
            break;
            
        default:
            handled = false;
            break;
    }
    
    return handled;

}

// -------------------------------------------------------------------------
void VSC::OB::Scene::initWorld(const Ogre::Vector3 &gravityVector, const Ogre::AxisAlignedBox &bounds)
{
    // Start Bullet
    mWorld = new DynamicsWorld (mSceneMgr, bounds, gravityVector, true, true, 10000);

    // add Debug info display tool
    DebugDrawer *debugDrawer = new DebugDrawer();

    mWorld->setDebugDrawer(debugDrawer);

    SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode("debugDrawer", Ogre::Vector3::ZERO);
    node->attachObject (static_cast <SimpleRenderable *> (debugDrawer));


}
// -------------------------------------------------------------------------
void VSC::OB::Scene::addGround()
{
    addStaticPlane(gStaticBodyRestitution, gStaticBodyFriction);
}
// -------------------------------------------------------------------------
OgreBulletDynamics::RigidBody* VSC::OB::Scene::addCube(const Ogre::String instanceName,
                                       const Ogre::Vector3 &pos, const Ogre::Quaternion &q, const Ogre::Vector3 &size,
                                       const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction, 
                                       const Ogre::Real bodyMass)
{
    /*
     *  Create an Ogre entity, and attach it to a node. Why no mention of size ?
     */
    
    Entity *entity = mSceneMgr->createEntity(instanceName + StringConverter::toString(mNumEntitiesInstanced), "Bulletbox.mesh");
    // "Crate.mesh", "Crate1.mesh", "Crate2.mesh"
    entity->setQueryFlags(GEOMETRY_QUERY_MASK);
	entity->setCastShadows(true);
    entity->setMaterialName("Bullet/box");
    SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject (entity);
    
    /*
     *  Create a corresponding OgreBulletCollisions::BoxCollisionShape which just represents the box shape
     *  and an OgreBulletDynamics::RigidBody which handles the Ogre::SceneNode, the OgreBulletCollisions::BoxCollisionShape,
     *  friction, mass, position and orientation (quaternion).
     */
    
    OgreBulletCollisions::BoxCollisionShape *sceneCubeShape = new OgreBulletCollisions::BoxCollisionShape(size);
    OgreBulletDynamics::RigidBody *defaultBody = new OgreBulletDynamics::RigidBody("defaultCubeRigid" + StringConverter::toString(mNumEntitiesInstanced), mWorld);
    defaultBody->setShape(node, sceneCubeShape, bodyRestitution, bodyFriction, bodyMass, pos, q);

    /*
     *  Push the objects in stl containers to track them. Perhaps move this to separate class ?
     */
    
    mEntities.push_back(entity);
    mShapes.push_back(sceneCubeShape);
    mBodies.push_back(defaultBody);
    
    mNumEntitiesInstanced++;

    return defaultBody;
}
// -------------------------------------------------------------------------
OgreBulletDynamics::RigidBody *VSC::OB::Scene::addSphere(const Ogre::String instanceName,
                                         const Ogre::Vector3 &pos, const Ogre::Quaternion &q, const Ogre::Real radius,
                                         const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction, 
                                         const Ogre::Real bodyMass)
{
    Entity *entity = mSceneMgr->createEntity(instanceName + StringConverter::toString(mNumEntitiesInstanced), "ellipsoid.mesh");
    entity->setQueryFlags (GEOMETRY_QUERY_MASK);
	entity->setCastShadows(true);
    entity->setMaterialName("Bullet/box");
    SceneNode *node = mSceneMgr->getRootSceneNode ()->createChildSceneNode ();
    node->attachObject (entity);
    
    SphereCollisionShape *sceneCubeShape = new SphereCollisionShape(radius);
    OgreBulletDynamics::RigidBody *defaultBody = new OgreBulletDynamics::RigidBody("defaultSphereRigid" + StringConverter::toString(mNumEntitiesInstanced), mWorld);
    defaultBody->setShape (node,  sceneCubeShape, bodyRestitution, bodyFriction, bodyMass, pos, q);

    mEntities.push_back(entity);
    mShapes.push_back(sceneCubeShape);
    mBodies.push_back(defaultBody);
    
    mNumEntitiesInstanced++;

    return defaultBody;
}

// -------------------------------------------------------------------------
OgreBulletDynamics::RigidBody *VSC::OB::Scene::addCylinder(const Ogre::String instanceName,
                                           const Ogre::Vector3 &pos, const Ogre::Quaternion &q, const Ogre::Vector3 &size,
                                           const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction, 
                                           const Ogre::Real bodyMass)
{
    Entity *entity = mSceneMgr->createEntity(instanceName + StringConverter::toString(mNumEntitiesInstanced), "cylinder.mesh");
    //   "capsule.mesh" "Barrel.mesh" "Barrel1.mesh" "Barrel2.mesh"
    entity->setQueryFlags (GEOMETRY_QUERY_MASK);
	entity->setCastShadows(true);
    entity->setMaterialName("Bullet/box");
    SceneNode *node = mSceneMgr->getRootSceneNode ()->createChildSceneNode ();
    node->attachObject (entity);
    
    CylinderCollisionShape *sceneCubeShape = new CylinderCollisionShape(size, Ogre::Vector3::UNIT_X);
    OgreBulletDynamics::RigidBody *defaultBody = new OgreBulletDynamics::RigidBody("defaultCylinderRigid" + StringConverter::toString(mNumEntitiesInstanced), mWorld);
    defaultBody->setShape (node, sceneCubeShape, bodyRestitution, bodyFriction, bodyMass, pos, q);

    mEntities.push_back(entity);
    mShapes.push_back(sceneCubeShape);
    mBodies.push_back(defaultBody);
    
    mNumEntitiesInstanced++;

    return defaultBody;
}
// -------------------------------------------------------------------------
OgreBulletDynamics::RigidBody *VSC::OB::Scene::addCone(const Ogre::String instanceName,
                                       const Ogre::Vector3 &pos, const Ogre::Quaternion &q, const Ogre::Vector3 &size,
                                       const Ogre::Real bodyRestitution, const Ogre::Real bodyFriction, 
                                       const Ogre::Real bodyMass)
{
    Entity *entity = mSceneMgr->createEntity(instanceName + StringConverter::toString(mNumEntitiesInstanced), "Mesh.mesh");
        //"Cone.mesh");

    entity->setQueryFlags (GEOMETRY_QUERY_MASK);
	entity->setCastShadows(true);
    entity->setMaterialName("Bullet/box");

    ConeCollisionShape *sceneCubeShape = new ConeCollisionShape(size.x, size.y, Ogre::Vector3::UNIT_Y);
    OgreBulletDynamics::RigidBody *defaultBody = new OgreBulletDynamics::RigidBody("defaultConeRigid" + StringConverter::toString(mNumEntitiesInstanced), mWorld);

    SceneNode *node = mSceneMgr->getRootSceneNode ()->createChildSceneNode ();
    node->attachObject (entity);
    defaultBody->setShape (node, sceneCubeShape, bodyRestitution, bodyFriction, bodyMass, pos, q);

    mEntities.push_back(entity);
    mShapes.push_back(sceneCubeShape);
    mBodies.push_back(defaultBody);
    mNumEntitiesInstanced++;

    return defaultBody;
}
// -------------------------------------------------------------------------
OgreBulletDynamics::RigidBody *VSC::OB::Scene::addStaticTrimesh(const Ogre::String &instanceName,
                                                const Ogre::String &meshName,
                                                const Ogre::Vector3 &pos, 
                                                const Ogre::Quaternion &q, 
                                                const Ogre::Real bodyRestitution, 
                                                const Ogre::Real bodyFriction,
                                                bool castShadow)
{
    Entity *sceneEntity = mSceneMgr->createEntity(instanceName + StringConverter::toString(mNumEntitiesInstanced), meshName);
    sceneEntity->setCastShadows (castShadow);

    StaticMeshToShapeConverter *trimeshConverter = new StaticMeshToShapeConverter(sceneEntity);
    TriangleMeshCollisionShape *sceneTriMeshShape = trimeshConverter->createTrimesh();
    delete trimeshConverter;
    
    OgreBulletDynamics::RigidBody *sceneRigid = new OgreBulletDynamics::RigidBody(instanceName + "Rigid" + StringConverter::toString(mNumEntitiesInstanced), mWorld);

    SceneNode *node = mSceneMgr->getRootSceneNode()->createChildSceneNode();
    node->attachObject (sceneEntity);

    sceneRigid->setStaticShape(node, sceneTriMeshShape, bodyRestitution, bodyFriction, pos);

    mEntities.push_back(sceneEntity);
    mBodies.push_back(sceneRigid);
    
    mNumEntitiesInstanced++;

    return sceneRigid;
}
// -------------------------------------------------------------------------
OgreBulletDynamics::RigidBody *VSC::OB::Scene::addStaticPlane( const Ogre::Real bodyRestitution, 
                                              const Ogre::Real bodyFriction)
{
    // Use a load of meshes to represent the floor
    int i = 0;
    StaticGeometry* s = mSceneMgr->createStaticGeometry("StaticFloor");
    s->setRegionDimensions(Ogre::Vector3(160.0, 100.0, 160.0));
    
    // Set the region origin so the center is at 0 world
    s->setOrigin(Ogre::Vector3::ZERO);
    
    for (Real z = -80.0; z <= 80.0; z += 20.0)
    {
        for (Real x = -80.0; x <= 80.0; x += 20.0)
        {
            String name = String("Plane_") + StringConverter::toString(i++);

            Entity* entity = mSceneMgr->createEntity(name, "plane.mesh");
			entity->setMaterialName("BulletPlane");
            entity->setQueryFlags (STATIC_GEOMETRY_QUERY_MASK);
            //entity->setUserObject(_plane);
            entity->setCastShadows(false);
            s->addEntity(entity, Ogre::Vector3(x,0,z));
        }
    }
    s->build();
    //SceneNode* mPlaneNode = mSceneMgr->getRootSceneNode()->createChildSceneNode(name);

    CollisionShape *Shape = new StaticPlaneCollisionShape (Ogre::Vector3(0,1,0), 0);

    OgreBulletDynamics::RigidBody *defaultPlaneBody = new OgreBulletDynamics::RigidBody("Plane" + StringConverter::toString(mNumEntitiesInstanced), mWorld);
    defaultPlaneBody->setStaticShape (Shape, bodyRestitution, bodyFriction);

    mBodies.push_back(defaultPlaneBody);
    mShapes.push_back(Shape);

    mNumEntitiesInstanced++;

    return defaultPlaneBody;
}


// -------------------------------------------------------------------------
void VSC::OB::Scene::updateStats(void)
{

    // update stats when necessary
    if (mFpsStaticText)
    {
        const RenderTarget::FrameStats& stats = mWindow->getStatistics();

        static String avgFps = "Average FPS: ";
        static String currFps = "Current FPS: ";
        static String tris = "Triangle Count: ";

        mFpsStaticText->setValue
            (
            avgFps + StringConverter::toString(stats.avgFPS) + " / " +
            currFps + StringConverter::toString(stats.lastFPS) + " / " +
            tris + StringConverter::toString(stats.triangleCount)
            );
    }

    try {
        OverlayElement* guiDbg = OverlayManager::getSingleton().getOverlayElement("Core/DebugText");
        guiDbg->setCaption(mDebugText);
    }
    catch(...) {}
}

