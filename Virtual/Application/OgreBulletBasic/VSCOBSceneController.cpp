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


//MARK: Basic Constructor which does abolutely nothing interesting at all 


VSC::OB::SceneController::SceneController() :
mShootSpeed (7.f),
mImpulseForce (10.f),
mDebugRayLine(0),
mRayQuery(0),
mPickConstraint(0),
mCollisionClosestRayResultCallback(0),
mOldPickingPosition(Ogre::Vector3(0,0,0)),
mOldPickingDistance(0)
{

}

// MARK Interface

//void VSC::OB::Scene::mouseButtonPressed(OIS::MouseButtonID buttonID)
bool VSC::OB::SceneController::mouseButtonPressed(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    
    if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseButtonPressed : " << position << " (" << buttonID << ")" << std::endl;
    
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
                if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseButtonPressed Left button, detected body" << std::endl;
                
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
                    
                    if (mTraceUI) {
                        std::cout << "VSC::OB::SceneController::mouseButtonPressed picked body " << mPickedBody;
                        std::cout << " at distance " << mOldPickingDist << std::endl;
                    }
                    
                    //very weak constraint for picking
                    p2p->setTau(0.1f);
                    mPickConstraint = p2p;
                    
                }
                
                getDebugLines();
                mDebugRayLine->addLine(rayTo.getOrigin(), pickPos);
                mDebugRayLine->draw();
                
            }
            
            else
            {
                if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseButtonPressed Left button, detected no body" << std::endl;
            }
            
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
                if (!(body->isStaticObject() || body->isKinematicObject()))
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
            if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseButtonPressed unknown mouse button pressed " << buttonID << std::endl;
            break;
        }
    }
    
    return VSC::OB::InputListener::mouseButtonPressed(renderWindow, position, buttonID);
}

bool VSC::OB::Scene::mouseButtonReleased(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    
    if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseButtonReleased : " << position << " (" << buttonID << ")" << std::endl;
    
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
            std::cout << "VSC::OB::SceneController::mouseButtonReleased unknown mouse button released " << buttonID << std::endl;
            break;
        }
    }
    
    return VSC::OB::InputListener::mouseButtonReleased(renderWindow, position, buttonID);
}



// -------------------------------------------------------------------------
bool VSC::OB::SceneController::mouseMoved(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, const Ogre::Vector2& movement)
{
    if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseMoved position: " << position << ", movement: " << movement << "" << std::endl;
    
    if (this->getInputAdapter()->isMouseButtonPressed(OIS::MB_Left))
    {
        if (mPickConstraint)
        {
            if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseMoved Left button is pressed and constraint exists" << std::endl;
            
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
            if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseMoved Left button is pressed and constraint does not exists" << std::endl;
        }
        
        return true;
    }

    return VSC::OB::InputListener::mouseMoved(renderWindow, position, movement);

}


bool VSC::OB::SceneController::mouseEntered(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position)
{
    if (mTraceUI) std::cout << "VSC::OB::SceneController mouse entered " << position << std::endl;
    
    return false;
}

bool VSC::OB::SceneController::mouseExited(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position)
{
    if (mTraceUI)  std::cout << "VSC::OB::SceneController mouse exited " << position << std::endl;
    
    return false;
}

// -------------------------------------------------------------------------
bool VSC::OB::SceneController::keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    
    if (mTraceUI) std::cout << "VSC::OB::SceneController got key pressed code: " << key << std::endl; 
    
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
                mDrawWireFrame = !mDrawWireFrame;
                if (mTraceUI) std::cout << "Wireframe is " << (mDrawWireFrame ? "on" : "off") << std::endl;
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
bool VSC::OB::SceneController::keyReleased(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    return VSC::OB::InputListener::keyReleased(renderWindow, key);
}

bool VSC::OB::SceneController::renderWindowChangedSize(Ogre::RenderWindow* renderWindow)
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

