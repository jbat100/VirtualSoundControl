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

#include "VSCOBSceneController.h"

#include "VSCOB.h"
#include "VSCOBApplication.h"
#include "VSCOBScene.h"
#include "VSCOBBetaGUIListener.h"
#include "VSCOBCameraController.h"
#include "VSCOBInputAdapter.h"
#include "VSCOBKeyboardAction.h"
#include "VSCOBDynamicObject.h"
#include "VSCOBBasicSceneElementFactory.h"

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
mRayQuery(0),
mPickConstraint(0),
mCollisionClosestRayResultCallback(0),
mOldPickingPosition(Ogre::Vector3(0,0,0)),
mOldPickingDistance(0)
{

}

VSC::OB::SceneController::SceneController(Scene::WPtr scene) :
mShootSpeed (7.f),
mImpulseForce (10.f),
mRayQuery(0),
mPickConstraint(0),
mCollisionClosestRayResultCallback(0),
mOldPickingPosition(Ogre::Vector3(0,0,0)),
mOldPickingDistance(0)
{
    setupWithScene(scene);
}

void VSC::OB::SceneController::setupWithScene(Scene::WPtr scene)
{
    this->shutdown();
    
    mScene = scene;
    
    Scene::SPtr s = mScene.lock();
    
    mRayQuery = s->getSceneManager()->createRayQuery(Ogre::Ray());
    mRayQuery->setQueryMask(VSC::OB::QueryMaskGeometry);
    mRayQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);
    Ogre::MovableObject::setDefaultQueryFlags(VSC::OB::QueryMaskAny);
    
    mPickConstraint = 0;
    
}

void VSC::OB::SceneController::shutdown()
{
    Scene::SPtr s = mScene.lock();
    
    s->getSceneManager()->destroyQuery(mRayQuery);
    mRayQuery = 0;
    
}

Ogre::Vector2 VSC::OB::SceneController::normalizedViewportCoordinates(const Ogre::Vector2& absCoord)
{
    Ogre::Viewport* viewport = this->getScene().lock()->getCamera()->getViewport();
    return Ogre::Vector2(absCoord.x / (float) viewport->getActualWidth(), 1.0 - (absCoord.y / (float) viewport->getActualHeight()));
}

// MARK Interface

//void VSC::OB::Scene::mouseButtonPressed(OIS::MouseButtonID buttonID)
bool VSC::OB::SceneController::mouseButtonPressed(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    
    if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseButtonPressed : " << position << " (" << buttonID << ")" << std::endl;
    
    Scene::SPtr scene = this->getScene().lock();
    
    switch (buttonID) 
    {
        case OIS::MB_Left:
        {
            // pick a body and try to drag it.
            Ogre::Vector3 pickPos;
            Ogre::Ray rayTo;
            
            /*
             *  Note this does not take into account the fact that one render window can contain multiple viewports
             *  and happily converts renderWindow coord to viewport coord directly.
             */
            
            Ogre::Viewport* viewport = scene->getCamera()->getViewport();
            
            Ogre::Vector2 pos = position;
            
            Scene::Element::WPtr e = scene->getElementAtViewportCoordinate(viewport, pos, pickPos, rayTo);
            Scene::Element::SPtr element = e.lock();
            
            if (element)
            {
                if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseButtonPressed Left button, detected element" << std::endl;
                
                OgreBulletDynamics::RigidBody* body = element->getRigidBody();
                
                if (!body->isStaticObject())
                {
                    mPickedBody = body;
                    mPickedBody->disableDeactivation();
                    
                    const Ogre::Vector3 localPivot (body->getCenterOfMassPivot(pickPos));
                    
                    OgreBulletDynamics::PointToPointConstraint *p2p  = new OgreBulletDynamics::PointToPointConstraint(body, localPivot);
                    scene->getDynamicsWorld()->addConstraint(p2p);
                    
                    //save mouse position for dragging
                    mOldPickingPosition = pickPos;
                    const Ogre::Vector3 eyePos(scene->getCamera()->getDerivedPosition());
                    mOldPickingDistance = (pickPos - eyePos).length();
                    
                    if (mTraceUI) {
                        std::cout << "VSC::OB::SceneController::mouseButtonPressed picked body " << mPickedBody;
                        std::cout << " at distance " << mOldPickingDistance << std::endl;
                    }
                    
                    //very weak constraint for picking
                    p2p->setTau(0.1f);
                    mPickConstraint = p2p;
                    
                }
                
                scene->getDebugRayLines()->addLine(rayTo.getOrigin(), pickPos);
                scene->getDebugRayLines()->draw();
                
            }
            
            else
            {
                if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseButtonPressed Left button, detected no element" << std::endl;
            }
            
            return true;
            
        }
            
        case OIS::MB_Middle:
        {
            // small unique impulse under cursor.
            
            Ogre::Vector3 pickPos;
            Ogre::Ray rayTo;
            
            /*
             *  Note this does not take into account the fact that one render window can contain multiple viewports
             *  and happily converts renderWindow coord to viewport coord directly.
             */
            
            Ogre::Viewport* viewport = scene->getCamera()->getViewport();
            Ogre::Vector2 pos = position;
            
            Scene::Element::WPtr e = scene->getElementAtViewportCoordinate(viewport, pos, pickPos, rayTo);
            Scene::Element::SPtr element = e.lock();
            
            if (element)
            {
                if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseButtonPressed Left button, detected element" << std::endl;
                
                OgreBulletDynamics::RigidBody* body = element->getRigidBody();
                
                if (!(body->isStaticObject() || body->isKinematicObject()))
                {
                    
                    body->enableActiveState ();
                    
                    const Ogre::Vector3 relPos (pickPos - body->getCenterOfMassPosition());
                    const Ogre::Vector3 impulse (rayTo.getDirection ());
                    
                    body->applyImpulse (impulse * mImpulseForce, relPos);		
                    
                }
                
                scene->getDebugRayLines()->addLine (rayTo.getOrigin(), pickPos);
                scene->getDebugRayLines()->draw();	
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

bool VSC::OB::SceneController::mouseButtonReleased(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    
    if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseButtonReleased : " << position << " (" << buttonID << ")" << std::endl;
    
    Scene::SPtr scene = this->getScene().lock();
    
    switch (buttonID) 
    {
        case OIS::MB_Left:
        {
            if (mPickConstraint)
            {
                // was dragging, but button released
                // Remove constraint
                scene->getDynamicsWorld()->removeConstraint(mPickConstraint);
                delete mPickConstraint;
                
                mPickConstraint = 0;
                mPickedBody->forceActivationState();
                mPickedBody->setDeactivationTime( 0.f );
                mPickedBody = 0;	
                
                scene->getDebugRayLines()->addLine (Ogre::Vector3::ZERO, Ogre::Vector3::ZERO);	
                scene->getDebugRayLines()->draw();  
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
    
    Scene::SPtr scene = (this->getScene()).lock();
    
    InputAdapter::SPtr adapter = this->getInputAdapter().lock();
    
    if (adapter->isMouseButtonPressed(OIS::MB_Left))
    {
        if (mPickConstraint)
        {
            if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseMoved Left button is pressed and constraint exists" << std::endl;
            
            Ogre::Viewport* viewport = scene->getCamera()->getViewport();
            float normX = position.x / (float) viewport->getActualWidth();
            float normY = 1.0 - (position.y / (float) viewport->getActualHeight());
            
            Ogre::Ray rayTo = scene->getCamera()->getCameraToViewportRay (normX, normY);
            
            //move the constraint pivot
            OgreBulletDynamics::PointToPointConstraint * p2p = static_cast <OgreBulletDynamics::PointToPointConstraint *>(mPickConstraint);
            
            //keep it at the same picking distance
            const Ogre::Vector3 eyePos(scene->getCamera()->getDerivedPosition());
            Ogre::Vector3 dir = rayTo.getDirection () * mOldPickingDistance;

            const Ogre::Vector3 newPos (eyePos + dir);
            p2p->setPivotB (newPos);    

            scene->getDebugRayLines()->addLine (mPickedBody->getWorldPosition (), newPos);
            scene->getDebugRayLines()->draw();
            
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
    
    Scene::SPtr scene = (this->getScene()).lock();
    
    InputAdapter::SPtr inputAdapter = this->getInputAdapter().lock();
    
    BOOST_ASSERT_MSG(inputAdapter, "Expected input adapter");
    
    if (!inputAdapter) {
        return false;
    }
    
    OIS::Keyboard::Modifier modifier = inputAdapter->getCurrentModifier();
    VSC::Keyboard::Combination comb(key, modifier);
    
    bool handled = true;
    
    const VSC::OB::KeyboardAction::KeySet& actionKeySet = this->getOgreKeyBindings()->getActionsForInput(comb);
    
    BOOST_FOREACH (VSC::OB::KeyboardAction::Key actionKey, actionKeySet) 
    {
        switch(actionKey)
        {
                
            case VSC::OB::KeyboardAction::SaveScreenShot:
                scene->getRenderWindow()->writeContentsToFile("OgreBulletScreenShot" + StringConverter::toString(count++) + ".png");
                break;
                
            // Scene Debug Options
                
            case VSC::OB::KeyboardAction::ToggleDisplayWireFrame:
                scene->toggleDrawWireFrame();
                if (mTraceUI) std::cout << "Wireframe is " << (scene->drawingWireFrame() ? "on" : "off") << std::endl;
                break;
                
            case VSC::OB::KeyboardAction::ToggleDisplayAABB:
                scene->toggleDrawAabb();
                if (mTraceUI) std::cout << "Draw AABB is " << (scene->drawingAabb() ? "on" : "off") << std::endl;
                break;
                
            case VSC::OB::KeyboardAction::ToggleFeaturesText:
                scene->toggleDrawFeaturesText();
                if (mTraceUI) std::cout << "Draw Features Text is " << (scene->drawingFeaturesText() ? "on" : "off") << std::endl;
                break;
                
            case VSC::OB::KeyboardAction::ToggleDisplayContactPoints:
                scene->toggleDrawContactPoints();
                if (mTraceUI) std::cout << "Draw contact points is " << (scene->drawingContactPoints() ? "on" : "off") << std::endl;
                break;
                
                /*
                
            case VSC::OB::KeyboardAction::ToggleDeactivation:
                mNoDeactivation = !mNoDeactivation;
                if (mTraceUI) std::cout << "No deactivation is " << (mNoDeactivation ? "on" : "off") << std::endl;
                break;
                
            case VSC::OB::KeyboardAction::ToggleHelpText:
                mNoHelpText = !mNoHelpText;
                if (mTraceUI) std::cout << "No help text is " << (mNoHelpText ? "on" : "off") << std::endl;
                break;
                 
                 */
                
            case VSC::OB::KeyboardAction::ToggleDrawText:
                scene->toggleDrawText();
                if (mTraceUI) std::cout << "Draw text is " << (scene->drawingText() ? "on" : "off") << std::endl;
                break;
                
                /*
                
            case VSC::OB::KeyboardAction::ToggleProfileTimings:
                mProfileTimings = !mProfileTimings;
                if (mTraceUI) std::cout << "Profile timings is " << (mProfileTimings ? "on" : "off") << std::endl;
                break;
                
            case VSC::OB::KeyboardAction::ToggleSatComparison:
                mEnableSatComparison = !mEnableSatComparison;
                if (mTraceUI) std::cout << "Enable sat comparison is " << (mEnableSatComparison ? "on" : "off") << std::endl;
                break;
                 
                 */
                
            case VSC::OB::KeyboardAction::ToggleBulletLCP:
                scene->toggleBulletLCP();
                if (mTraceUI) std::cout << "Disable bullet LCP is " << (scene->bulletLCPIsEnabled() ? "on" : "off") << std::endl;
                break;
                
            case VSC::OB::KeyboardAction::ToggleCCD:
                scene->toggleCCD();
                if (mTraceUI) std::cout << "Enable CCD is " << (scene->ccdIsEnabled() ? "on" : "off") << std::endl;
                break;
                
                // pause
            case VSC::OB::KeyboardAction::ToggleSimulationPause:
                scene->togglePause();
                if (mTraceUI) std::cout << "Paused is " << (scene->isPaused() ? "on" : "off") << std::endl;
                break;
                
                // single step
            case VSC::OB::KeyboardAction::SimulationStep:
                scene->doOneStep();;
                if (mTraceUI) std::cout << "Doing one step" << std::endl;
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
                
            case VSC::OB::KeyboardAction::ShootCube:
                this->throwDynamicObjectPrimitive(VSC::OB::PrimitiveCube,
                                                  scene->getCamera()->getDerivedDirection().normalisedCopy()*mShootSpeed);
                break;
                
            case VSC::OB::KeyboardAction::ShootSphere:
                this->throwDynamicObjectPrimitive(VSC::OB::PrimitiveSphere,
                                                  scene->getCamera()->getDerivedDirection().normalisedCopy()*mShootSpeed);
                break;
                
            case VSC::OB::KeyboardAction::ShootCylinder:
                this->throwDynamicObjectPrimitive(VSC::OB::PrimitiveCylinder,
                                                  scene->getCamera()->getDerivedDirection().normalisedCopy()*mShootSpeed);
                break;
                
            case VSC::OB::KeyboardAction::ShootCone:
                this->throwDynamicObjectPrimitive(VSC::OB::PrimitiveCone,
                                                  scene->getCamera()->getDerivedDirection().normalisedCopy()*mShootSpeed);
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
    Scene::SPtr scene = this->getScene().lock();
    
    if (renderWindow == scene->getRenderWindow())
    {
        scene->resetCameraAspectRatio();
        return true;
    }
    
    return false;
}

// -------------------------------------------------------------------------
void VSC::OB::SceneController::throwDynamicObjectPrimitive(VSC::OB::PrimitiveType primitiveType, const Ogre::Vector3& velocity)
{
    
    const float throwDist = 2.0f;
    
    Scene::SPtr scene = this->getScene().lock();
    
    if (scene->checkIfEnoughPlaceToAddObject(throwDist) == false)
    {
        // TODO throw exception ?
        return; // false;
    }
    
    bool handled = true;
    
    BasicSceneElementFactory::SPtr sceneFactory = boost::dynamic_pointer_cast<BasicSceneElementFactory> (scene->getElementFactory());
    
    VSC::OB::DynamicObject::FactoryDescription description;
    
    description.position = scene->getCamera()->getDerivedPosition();
    
    VSC::OB::DynamicObject::WPtr object;
    
    switch(primitiveType)
    {
        case VSC::OB::PrimitiveCube:            
            description.name = "Cube";
            description.size = gCubeBodyBounds;
            object = sceneFactory->addPrimitive(VSC::OB::PrimitiveCube, description);
            break;
            
        case VSC::OB::KeyboardAction::ShootSphere: 
            description.name = "Sphere";
            description.size = gSphereBodyBounds;
            object = sceneFactory->addPrimitive(VSC::OB::PrimitiveSphere, description);
            break;
            
        case VSC::OB::KeyboardAction::ShootCylinder: 
            description.name = "Cylinder";
            description.size = gCylinderBodyBounds;
            object = sceneFactory->addPrimitive(VSC::OB::PrimitiveCylinder, description);
            break;
            
        case VSC::OB::KeyboardAction::ShootCone: 
            description.name = "Cone";
            description.size = gConeBodyBounds;
            object = sceneFactory->addPrimitive(VSC::OB::PrimitiveCylinder, description);
            break;
            
        default:
            handled = false;
            break;
    }
    
    VSC::OB::DynamicObject::SPtr sObject = object.lock();
    
    if (sObject)
    {
        sObject->getRigidBody()->setLinearVelocity(velocity);
    }

}



