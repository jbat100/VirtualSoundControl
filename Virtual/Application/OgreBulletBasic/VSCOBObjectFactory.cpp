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

//MARK: Convert Shadow technique to int and back, local functions, only used here

// -------------------------------------------------------------------------
Ogre::ShadowTechnique convertToShadowTechnique(int i)
{
    switch(i)
    {
    case 0: return SHADOWTYPE_NONE;
    case 1: return SHADOWDETAILTYPE_ADDITIVE;
    case 2: return SHADOWDETAILTYPE_MODULATIVE;
    case 3: return SHADOWDETAILTYPE_INTEGRATED;
    case 4: return SHADOWDETAILTYPE_STENCIL;
    case 5: return SHADOWDETAILTYPE_TEXTURE;
    case 6: return SHADOWTYPE_STENCIL_MODULATIVE;
    case 7: return SHADOWTYPE_STENCIL_ADDITIVE;
    case 8: return SHADOWTYPE_TEXTURE_MODULATIVE;
    case 9: return SHADOWTYPE_TEXTURE_ADDITIVE;
    case 10: return SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED;
    case 11: return SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED;
    default: return SHADOWTYPE_NONE;
    }
}
// -------------------------------------------------------------------------
int convertShadowTechniqueToInt(Ogre::ShadowTechnique i)
{
	switch(i)
	{
	case  SHADOWTYPE_NONE: return 0;
	case  SHADOWDETAILTYPE_ADDITIVE: return 1;
	case  SHADOWDETAILTYPE_MODULATIVE: return 2;
	case  SHADOWDETAILTYPE_INTEGRATED: return 3;
	case  SHADOWDETAILTYPE_STENCIL: return 4;
	case  SHADOWDETAILTYPE_TEXTURE: return 5;
	case  SHADOWTYPE_STENCIL_MODULATIVE: return 6;
	case  SHADOWTYPE_STENCIL_ADDITIVE: return 7;
	case  SHADOWTYPE_TEXTURE_MODULATIVE: return 8;
	case  SHADOWTYPE_TEXTURE_ADDITIVE: return 9;
	case  SHADOWTYPE_TEXTURE_ADDITIVE_INTEGRATED: return 10;
	case  SHADOWTYPE_TEXTURE_MODULATIVE_INTEGRATED: return 11;
	default: return 0;
	}
}

//MARK: Basic Constructor which does abolutely nothing interesting at all 

VSC::OB::Scene::Scene() :    
mCamera(0),
mRoot(0),
mSceneMgr(0),
mWindow(0),
mWorld(0),
mStatsOn (true),
mPaused (false),
mShootSpeed (7.f),
mImpulseForce (10.f),
mDebugRayLine(0),
mRayQuery(0),
mGuiListener(0),
mPickConstraint(0),
mCollisionClosestRayResultCallback(0)
{

}
// -------------------------------------------------------------------------
void VSC::OB::Scene::init(Ogre::Root *root, Ogre::RenderWindow *win, VSC::OB::Application *application)
{
    /*
     *  A bit of OGRE reading ...
     *  - Ogre::Root http://www.ogre3d.org/docs/api/html/classOgre_1_1Root.html
     *  - Ogre::RenderWindow http://www.ogre3d.org/docs/api/html/classOgre_1_1RenderWindow.html
     */
    
    mRoot = root;
    mWindow = win; 
    mApplication = application;
    
    /*
     *  IMPORTANT: Subclasses must have created a camera before calling VSC::OB::Scene::init
     */
    this->setCameraController(VSC::OB::CameraController::SPtr(new VSC::OB::CameraController));
    this->getCameraController()->setCamera(mCamera);
    this->getCameraController()->setCameraSpeed(0.3f);

    /**
     *  We have separate GUI and Input listeners, presumably to keep this agnostic to the interface type
     */
    mGuiListener = new VSC::OB::BetaGUIListener(this, win);

    /******************* CREATESHADOWS If not debug mode ***************************/
#ifndef _DEBUG
    
    mCurrentShadowTechnique = convertShadowTechniqueToInt(SHADOWTYPE_TEXTURE_ADDITIVE);

    /**
     *  QUESTION: Where does mSceneMgr get set to something other than 0? ANSWER: In the subclasses (Demos) of course.
     *  NOTE: Ogre::SceneManager is an absolutely massive class http://www.ogre3d.org/docs/api/html/classOgre_1_1SceneManager.html
     */
	mSceneMgr->setShadowColour(ColourValue(0.5, 0.5, 0.5));

	Ogre::PixelFormat pxlFmt = Ogre::PF_L8;
	if (mRoot->getRenderSystem()->getCapabilities()->hasCapability(RSC_TEXTURE_FLOAT))
	{
		const bool isOpenGL = (Ogre::Root::getSingleton().getRenderSystem()->getName().find("GL") != Ogre::String::npos);
		if (isOpenGL)// GL performs much better if you pick half-float format
			pxlFmt = Ogre::PF_FLOAT16_R;
		else
			pxlFmt = Ogre::PF_FLOAT32_R;// D3D is the opposite - if you ask for PF_FLOAT16_R you
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

	mSceneMgr->setShadowTechnique(convertToShadowTechnique(mCurrentShadowTechnique));
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
    
    mRayQuery->setQueryMask(GEOMETRY_QUERY_MASK);
    mRayQuery->setQueryTypeMask(SceneManager::ENTITY_TYPE_MASK);
    
    MovableObject::setDefaultQueryFlags (ANY_QUERY_MASK);

    mPickConstraint = 0;
    
    /**
     *  Nothing is enabled by default...
     */
    
    mQuit = false;
    mPaused = false;
    mWireFrame = false;
    mDrawAabb = false;
    mDrawFeaturesText = false;
    mDrawContactPoints = false;
    mNoDeactivation = false;
    mNoHelpText = false;
    mDrawText = false;
    mProfileTimings = false;
    mEnableSatComparison = false;
    mDisableBulletLCP = false;
    mEnableCCD = false;

}



void VSC::OB::Scene::setCameraController(VSC::OB::CameraController::SPtr controller)
{
    mCameraController = controller;
    this->setNextInputListener(mCameraController.get());
    mCameraController->setOgreKeyBindings(this->getOgreKeyBindings());
}

// -------------------------------------------------------------------------
void VSC::OB::Scene::setBasicLight()
{
    // Set ambient light
    mSceneMgr->setAmbientLight(ColourValue(0.4, 0.4, 0.4));

	// Fixed light, dim
 	mSunLight = mSceneMgr->createLight("Sun");
 	mSunLight->setPosition(0.0, 30.5, 0.0);
 	mSunLight->setCastShadows(false);
 	mSunLight->setType(Light::LT_POINT);
 	mSunLight->setDiffuseColour(g_minLightColour);
 	mSunLight->setSpecularColour(0.2, 0.2, 0.2);
 	mSunLight->setAttenuation(8000, 1, 0.0005, 0);

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


void VSC::OB::Scene::setPhysicGUI()
{
    BetaGUI::GUI *gui = mGuiListener->getGui();

    BetaGUI::Window *menuWindow = gui->addMenuWindow(Vector2(mWindow->getWidth(), 24));
    menuWindow->hide ();

    BetaGUI::Window *aWindow;
     
    /**-------------------------------------------------------------------
     *  Time tab, play/pause, step
     */
    aWindow = menuWindow->addMenuWindowTab("Time", false, BetaGUI::WPT_NONE);
    aWindow->addBoolButton(&mPaused, "Play/Pause", BetaGUI::WPT_HORIZONTAL);
    aWindow->addBoolButton(&mDoOnestep, "Single Step", BetaGUI::WPT_HORIZONTAL);
    aWindow->hide ();

    /**-------------------------------------------------------------------
     *  Setup effect for appears and slide in
     */
    const Vector2 screenRightTop (mWindow->getWidth () - aWindow->getSize ().x, 0);
    const Vector2 screenRightOffTop (mWindow->getWidth () - aWindow->getSize ().x, - aWindow->getSize ().y);
    gui->addEffect(new BetaGUI::MoveEffect(aWindow, 2, screenRightOffTop, screenRightTop, 0));
    gui->addEffect(new BetaGUI::AlphaEffect(aWindow, 2, 0, 1, 0));

    /**-------------------------------------------------------------------
     *  Interaction tab
     */
    aWindow = menuWindow->addMenuWindowTab("Interaction");
    aWindow->addRealButton(&mShootSpeed,
        Vector4(0.1, 5.0, 0.0, 100.0),
        "Shoot Speed:", BetaGUI::WPT_VERTICAL);
    aWindow->addRealButton(&mImpulseForce,
        Vector4(0.1, 5.0, 0.0, 100.0),
        "Impulse Force:", BetaGUI::WPT_VERTICAL);
    aWindow->hide ();

    /**-------------------------------------------------------------------
     *  Debug tab
     */
    aWindow = menuWindow->addMenuWindowTab("Debug");
    aWindow->addBoolButton(&mWireFrame, "Draw Wireframe", BetaGUI::WPT_VERTICAL);
    aWindow->addBoolButton(&mDrawAabb, "Draw Aabb", BetaGUI::WPT_VERTICAL);
    aWindow->addBoolButton(&mDrawFeaturesText, "Draw Features Text", BetaGUI::WPT_VERTICAL);
    aWindow->addBoolButton(&mDrawContactPoints, "Draw Contact Points", BetaGUI::WPT_VERTICAL);
    aWindow->addBoolButton(&mNoDeactivation, "No Deactivation", BetaGUI::WPT_VERTICAL);
    aWindow->addBoolButton(&mNoHelpText, "No Help Text", BetaGUI::WPT_VERTICAL);
    aWindow->addBoolButton(&mDrawText, "Draw Text", BetaGUI::WPT_VERTICAL);
    aWindow->addBoolButton(&mProfileTimings, "Profile Timings", BetaGUI::WPT_VERTICAL);
    aWindow->addBoolButton(&mEnableSatComparison, "Enable Sat Comparison", BetaGUI::WPT_VERTICAL);
    aWindow->addBoolButton(&mDisableBulletLCP, "Disable Bullet LCP", BetaGUI::WPT_VERTICAL);
    aWindow->addBoolButton(&mEnableCCD, "Enable CCD", BetaGUI::WPT_VERTICAL);
    aWindow->hide ();

    /**-------------------------------------------------------------------
     *  FPS tab
     */
    aWindow = menuWindow ->addMenuWindowTab("FPS", false, BetaGUI::WPT_NONE);
    mFpsStaticText = aWindow->addStaticText("FPS Count", BetaGUI::WPT_VERTICAL);
    aWindow->hide ();
    // appears and slide in
    const Vector2 screenRightBottom (mWindow->getWidth () - 360, mWindow->getHeight () - 24);
    const Vector2 screenRightOffBottom (mWindow->getWidth () - 360, mWindow->getHeight ());
    gui->addEffect(new BetaGUI::MoveEffect(aWindow, 2, screenRightOffBottom, screenRightBottom, 0));
    gui->addEffect(new BetaGUI::AlphaEffect(aWindow, 2, 0, 1, 0));

    /**-------------------------------------------------------------------
     *  Help tab
     */
    aWindow = menuWindow->addMenuWindowTab("Help");
    aWindow->addStaticText(mName + " Help Informations", BetaGUI::WPT_VERTICAL);
    std::vector <String>::iterator keyIterator = mHelpKeys.begin();
    for (; keyIterator < mHelpKeys.end(); ++keyIterator)
    {
        aWindow->addStaticText(*keyIterator, BetaGUI::WPT_VERTICAL); 
    }
    // appears and slide
    const Vector2 halfWindowSize (aWindow->getSize ().x / 2, aWindow->getSize ().y / 2);
    const Vector2 screenCentered ((mWindow->getWidth () / 2) - halfWindowSize.x,
        (mWindow->getHeight () / 2) - halfWindowSize.y);
    gui->addEffect(new BetaGUI::MoveEffect(aWindow, 2, -halfWindowSize, screenCentered, 0));
    gui->addEffect(new BetaGUI::AlphaEffect(aWindow, 2, 0, 1, 0));
    // disappears
    gui->addEffect(new BetaGUI::AlphaEffect(aWindow, 2, 1, 0, 3));

    /**-------------------------------------------------------------------
     *  Quit button
     */
    menuWindow->addBoolButton(&mQuit, "Quit", BetaGUI::WPT_HORIZONTAL);
    // appears and slide in
    const Vector2 screenLeftTop (- menuWindow->getSize ().x, 0);
    const Vector2 screenLeftOffTop (0, 0);
    gui->addEffect(new BetaGUI::MoveEffect(menuWindow, 2, screenLeftTop, screenLeftOffTop, 0));
    gui->addEffect(new BetaGUI::AlphaEffect(menuWindow, 2, 0, 1, 0));
}


void VSC::OB::Scene::getDebugLines()
{
    if (mDebugRayLine == 0)
    {
        mDebugRayLine = new DebugLines();
        mSceneMgr->getRootSceneNode ()->createChildSceneNode ()->attachObject (mDebugRayLine);
    }
}


void VSC::OB::Scene::shutdown ()
{

    delete mGuiListener;
    mGuiListener = 0;

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

// MARK Interface


//void VSC::OB::Scene::mouseButtonPressed(OIS::MouseButtonID buttonID)
bool VSC::OB::Scene::mouseButtonPressed(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    
    if (mTraceUI) std::cout << "VSC::OB::Scene::mouseButtonPressed : " << position << " (" << buttonID << ")" << std::endl;
    
    switch (buttonID) 
    {
        case OIS::MB_Left:
        {
            // pick a body and try to drag it.
            Ogre::Vector3 pickPos;
            Ogre::Ray rayTo;
            
            OgreBulletDynamics::RigidBody* body = getBodyUnderCursorUsingBullet(pickPos, rayTo);
            //OgreBulletDynamics::RigidBody* body = getBodyUnderCursorUsingOgre(pickPos, rayTo);
            
            if (body)
            {
                if (mTraceUI) std::cout << "VSC::OB::Scene::mouseButtonPressed Left button, detected body" << std::endl;
                
                if (!body->isStaticObject())
                {
                    mPickedBody = body;
                    mPickedBody->disableDeactivation();		
                    const Ogre::Vector3 localPivot (body->getCenterOfMassPivot(pickPos));
                    
                    OgreBulletDynamics::PointToPointConstraint *p2p  = new OgreBulletDynamics::PointToPointConstraint(body, localPivot);
                    mWorld->addConstraint(p2p);					    
                    
                    //save mouse position for dragging
                    mOldPickingPos = pickPos;
                    const Ogre::Vector3 eyePos(mCamera->getDerivedPosition());
                    mOldPickingDist = (pickPos - eyePos).length();
                    
                    if (mTraceUI) std::cout << "VSC::OB::Scene::mouseButtonPressed picked body " << mPickedBody << " at distance " << mOldPickingDist << std::endl;
                    
                    //very weak constraint for picking
                    p2p->setTau (0.1f);
                    mPickConstraint = p2p;
                    
                }
                getDebugLines();
                mDebugRayLine->addLine(rayTo.getOrigin(), pickPos);
                mDebugRayLine->draw();
                
            }
            
            else
            {
                if (mTraceUI) std::cout << "VSC::OB::Scene::mouseButtonPressed Left button, detected no body" << std::endl;
            }
            
            /*
             *  Beta mouse hidden... 
            if (mGuiListener->getGui()->injectMouse(position.x*mWindow->getWidth(), position.y*mWindow->getHeight(), true))
            {
                //mGuiListener->hideMouse();
            }
            else
            {
                //mGuiListener->showMouse ();
            }
             */
            
            return true;
            
        }
            
        case OIS::MB_Middle:
        {
            // small unique impulse under cursor.
            Ogre::Vector3 pickPos;
            Ogre::Ray rayTo;
            OgreBulletDynamics::RigidBody * body = getBodyUnderCursorUsingBullet(pickPos, rayTo);
            //getBodyUnderCursorUsingOgre(pickPos, rayTo);
            if (body)
            {  
                if (!(body->isStaticObject() 
                      || body->isKinematicObject()
                      ))
                {
                    
                    body->enableActiveState ();
                    
                    const Ogre::Vector3 relPos (pickPos - body->getCenterOfMassPosition());
                    const Ogre::Vector3 impulse (rayTo.getDirection ());
                    
                    body->applyImpulse (impulse * mImpulseForce, relPos);		
                    
                }
                
                getDebugLines();
                mDebugRayLine->addLine (rayTo.getOrigin(), pickPos);
                mDebugRayLine->draw();	
            }
            
            return true;
        }
            

        case OIS::MB_Right:
        {
            break;
        }
            
        default:
        {
            //std::cout << "Unknown mouse button pressed " << buttonID << std::endl;
            break;
        }
    }
    
    return VSC::OB::InputListener::mouseButtonPressed(renderWindow, position, buttonID);
}

bool VSC::OB::Scene::mouseButtonReleased(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    
    if (mTraceUI) std::cout << "VSC::OB::Scene mouseButtonReleased : " << position << " (" << buttonID << ")" << std::endl;
    
    switch (buttonID) 
    {
        case OIS::MB_Left:
        {
            if (mPickConstraint)
            {
                // was dragging, but button released
                // Remove constraint
                mWorld->removeConstraint(mPickConstraint);
                delete mPickConstraint;
                
                mPickConstraint = 0;
                mPickedBody->forceActivationState();
                mPickedBody->setDeactivationTime( 0.f );
                mPickedBody = 0;	
                
                getDebugLines();
                mDebugRayLine->addLine (Ogre::Vector3::ZERO, Ogre::Vector3::ZERO);	
                mDebugRayLine->draw();  
                mGuiListener->showMouse(); 
            }
            
            return true;
        }
            
        case OIS::MB_Middle:
        {
            return true; // middle button is handled by scene for shooting
        }
            
        case OIS::MB_Right:
        {
            break;
        }
            
        default:
        {
            std::cout << "Unknown mouse button released " << buttonID << std::endl;
            break;
        }
    }
    
    return VSC::OB::InputListener::mouseButtonReleased(renderWindow, position, buttonID);
}



// -------------------------------------------------------------------------
bool VSC::OB::Scene::mouseMoved(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, const Ogre::Vector2& movement)
{
    if (mTraceUI) std::cout << "VSC::OB::Scene::mouseMoved position: " << position << ", movement: " << movement << "" << std::endl;
    
    if (this->getInputAdapter()->isMouseButtonPressed(OIS::MB_Left))
    {
        if (mPickConstraint)
        {
            if (mTraceUI) std::cout << "VSC::OB::Scene::mouseMoved Left button is pressed and constraint exists" << std::endl;
            
            Ogre::Viewport* viewport = mCamera->getViewport();
            float normX = position.x / (float) viewport->getActualWidth();
            float normY = 1.0 - (position.y / (float) viewport->getActualHeight());
            
            Ogre::Ray rayTo = mCamera->getCameraToViewportRay (normX, normY);
            
            //move the constraint pivot
            OgreBulletDynamics::PointToPointConstraint * p2p = static_cast <OgreBulletDynamics::PointToPointConstraint *>(mPickConstraint);
            
            //keep it at the same picking distance
            const Ogre::Vector3 eyePos(mCamera->getDerivedPosition());
            Ogre::Vector3 dir = rayTo.getDirection () * mOldPickingDist;

            const Ogre::Vector3 newPos (eyePos + dir);
            p2p->setPivotB (newPos);    

            setDebugText ("Dragging");

            getDebugLines();
            mDebugRayLine->addLine (mPickedBody->getWorldPosition (), newPos);
            mDebugRayLine->draw();
            
        }
        
        else
        {
            if (mTraceUI) std::cout << "VSC::OB::Scene::mouseMoved Left button is pressed and constraint does not exists" << std::endl;
        }
        
        return true;
    }

    return VSC::OB::InputListener::mouseMoved(renderWindow, position, movement);

    /*
    
    if (mGuiListener->getGui()->injectMouse(position.x * mWindow->getWidth(), position.y * mWindow->getHeight(), 
                                            this->getMouseAdapter()->isMouseButtonPressed(OIS::MB_Left)))
    {
        //mGuiListener->hideMouse();
    }
    
    else 
    {
        //mGuiListener->showMouse();
    }
     
     */

}


bool VSC::OB::Scene::mouseEntered(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position)
{
    if (mTraceUI) std::cout << "VSC::OB::Scene mouse entered " << position << std::endl;
    
    return false;
}

bool VSC::OB::Scene::mouseExited(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position)
{
    if (mTraceUI)  std::cout << "VSC::OB::Scene mouse exited " << position << std::endl;
    
    return false;
}

// -------------------------------------------------------------------------
bool VSC::OB::Scene::keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    
    if (mTraceUI) std::cout << "VSC::OB::Scene got key pressed code: " << key << std::endl; 
    
    static int count = 0;
    // Scene Debug Options
    
    OIS::Keyboard::Modifier modifier = this->getInputAdapter()->getCurrentModifier();
    VSC::Keyboard::Combination comb(key, modifier);
    
    bool handled = true;
    
    const VSC::OB::KeyboardAction::KeySet& actionKeySet = this->getOgreKeyBindings()->getActionsForInput(comb);
    
    BOOST_FOREACH (VSC::OB::KeyboardAction::Key actionKey, actionKeySet) 
    {
        switch(actionKey)
        {
            // Application Utils
            
            case VSC::OB::KeyboardAction::Quit:
                mQuit = true;
                break;
                
            case VSC::OB::KeyboardAction::SaveScreenShot:
                mWindow->writeContentsToFile("OgreBulletScreenShot"+StringConverter::toString(count++)+".png");
                break;
                
            // Scene Debug Options
                
            case VSC::OB::KeyboardAction::ToggleDisplayWireFrame:
                mWireFrame = !mWireFrame;
                if (mTraceUI) std::cout << "Wireframe is " << (mWireFrame ? "on" : "off") << std::endl;
                break;
            case VSC::OB::KeyboardAction::ToggleDisplayAABB:
                mDrawAabb = !mDrawAabb;
                if (mTraceUI) std::cout << "Draw AABB is " << (mDrawAabb ? "on" : "off") << std::endl;
                break;
            case VSC::OB::KeyboardAction::ToggleFeaturesText:
                mDrawFeaturesText = !mDrawFeaturesText;
                if (mTraceUI) std::cout << "Draw Features Text is " << (mDrawFeaturesText ? "on" : "off") << std::endl;
                break;
            case VSC::OB::KeyboardAction::ToggleDisplayContactPoints:
                mDrawContactPoints = !mDrawContactPoints;
                if (mTraceUI) std::cout << "Draw contact points is " << (mDrawContactPoints ? "on" : "off") << std::endl;
                break;
            case VSC::OB::KeyboardAction::ToggleDeactivation:
                mNoDeactivation = !mNoDeactivation;
                if (mTraceUI) std::cout << "No deactivation is " << (mNoDeactivation ? "on" : "off") << std::endl;
                break;
            case VSC::OB::KeyboardAction::ToggleHelpText:
                mNoHelpText = !mNoHelpText;
                if (mTraceUI) std::cout << "No help text is " << (mNoHelpText ? "on" : "off") << std::endl;
                break;
            case VSC::OB::KeyboardAction::ToggleDrawText:
                mDrawText = !mDrawText;
                if (mTraceUI) std::cout << "Draw text is " << (mDrawText ? "on" : "off") << std::endl;
                break;
            case VSC::OB::KeyboardAction::ToggleProfileTimings:
                mProfileTimings = !mProfileTimings;
                if (mTraceUI) std::cout << "Profile timings is " << (mProfileTimings ? "on" : "off") << std::endl;
                break;
            case VSC::OB::KeyboardAction::ToggleSatComparison:
                mEnableSatComparison = !mEnableSatComparison;
                if (mTraceUI) std::cout << "Enable sat comparison is " << (mEnableSatComparison ? "on" : "off") << std::endl;
                break;
            case VSC::OB::KeyboardAction::ToggleBulletLCP:
                mDisableBulletLCP = !mDisableBulletLCP;
                if (mTraceUI) std::cout << "Disable bullet LCP is " << (mDisableBulletLCP ? "on" : "off") << std::endl;
                break;
            case VSC::OB::KeyboardAction::ToggleCCD:
                mEnableCCD = !mEnableCCD;
                if (mTraceUI) std::cout << "Enable CCD is " << (mEnableCCD ? "on" : "off") << std::endl;
                break;
                
                // pause
            case VSC::OB::KeyboardAction::ToggleSimulationPause:
                mPaused = !mPaused;
                if (mTraceUI) std::cout << "Paused is " << (mPaused ? "on" : "off") << std::endl;
                break;
                // single step
            case VSC::OB::KeyboardAction::SimulationStep:
                mDoOnestep = !mDoOnestep;
                if (mTraceUI) std::cout << "Do one step is " << (mDoOnestep ? "on" : "off") << std::endl;
                break;
                // faster Shoots
            case VSC::OB::KeyboardAction::IncrementShootSpeed:
                mShootSpeed += 5.0f;
                if (mTraceUI) std::cout << "Shoot speed is " << mShootSpeed << std::endl;
                break;
                // Slower Shoots
            case VSC::OB::KeyboardAction::DecrementShootSpeed:
                mShootSpeed -= 5.0f;
                if (mTraceUI) std::cout << "Shoot speed is " << mShootSpeed << std::endl;
                break;
                
            default:
                handled = false;
                break;
        }
    }
    
    if (handled) return true;
    
    return VSC::OB::InputListener::keyPressed(renderWindow, key);
}

// -------------------------------------------------------------------------
bool VSC::OB::Scene::keyReleased(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    return VSC::OB::InputListener::keyReleased(renderWindow, key);
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
    if (mWireFrame)
    {
        const bool wasWireframeShapes = debugDrawer->doesDrawWireframe();
        debugDrawer->setDrawWireframe(!wasWireframeShapes);
        mWorld->setShowDebugShapes(!wasWireframeShapes);
        mWireFrame = false;
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

