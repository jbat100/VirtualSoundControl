
#include "VSCOBSceneController.h"

#include "VSCOB.h"
#include "VSCOBApplication.h"
#include "VSCOBScene.h"
#include "VSCOBInterface.h"
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

void VSC::OB::SceneController::setupWithScene(Scene::SPtr scene)
{
    this->shutdown();
    
    mScene = Scene::WPtr(scene);
    
    Scene::SPtr s = mScene.lock();
    
    mRayQuery = s->getSceneManager()->createRayQuery(Ogre::Ray());
    mRayQuery->setQueryMask(VSC::OB::QueryMaskGeometry);
    mRayQuery->setQueryTypeMask(Ogre::SceneManager::ENTITY_TYPE_MASK);
    Ogre::MovableObject::setDefaultQueryFlags(VSC::OB::QueryMaskAny);
    
    mPickConstraint = 0;
    
}

void VSC::OB::SceneController::shutdown()
{
    Scene::SPtr s = this->getScene();
    
    BOOST_ASSERT(this->getScene());
    
    if (s)
    {
        s->getSceneManager()->destroyQuery(mRayQuery);
        mRayQuery = 0;
    }
    
}


// MARK Interface

//void VSC::OB::Scene::mouseButtonPressed(OIS::MouseButtonID buttonID)
bool VSC::OB::SceneController::mouseButtonPressed(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    
    if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseButtonPressed : " << position << " (" << buttonID << ")" << std::endl;
    
    Scene::SPtr scene = this->getScene();
    BOOST_ASSERT(scene);
    Display::SPtr display = scene->getApplication()->getDisplayWithRenderWindow(renderWindow);
    
    switch (buttonID) 
    {
        case OIS::MB_Left:
        {
            if (!display) break;
            
            // pick a body and try to drag it.
            Ogre::Vector3 pickPos;
            Ogre::Ray rayTo;
            
            /*
             *  Note this does not take into account the fact that one render window can contain multiple viewports
             *  and happily converts renderWindow coord to viewport coord directly.
             */
            
            Ogre::Viewport* v = display->getCamera()->getViewport();
            
            Ogre::Vector2 pos = position;
            Ogre::Vector2 normPos = Ogre::Vector2( pos.x / v->getActualWidth(), 1.0 - (pos.y / v->getActualHeight()) );
            
            Scene::Element::SPtr element = scene->getElementAtDisplayCoordinate(display, normPos, pickPos, rayTo);
            
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
                    const Ogre::Vector3 eyePos(display->getCamera()->getDerivedPosition());
                    mOldPickingDistance = (pickPos - eyePos).length();
                    
                    if (mTraceUI) {
                        std::cout << "VSC::OB::SceneController::mouseButtonPressed picked body " << mPickedBody;
                        std::cout << " at distance " << mOldPickingDistance << std::endl;
                    }
                    
                    //very weak constraint for picking
                    p2p->setTau(0.1f);
                    mPickConstraint = p2p;
                    
                }
                
                scene->getDebugLines()->addLine(rayTo.getOrigin(), pickPos);
                scene->getDebugLines()->draw();
                
            }
            
            else
            {
                if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseButtonPressed Left button, detected no element" << std::endl;
            }
            
            return true;
            
        }
            
        case OIS::MB_Middle:
        {
            if (!display) break;
            
            // small unique impulse under cursor.
            Ogre::Vector3 pickPos;
            Ogre::Ray rayTo;
            
            /*
             *  Note this does not take into account the fact that one render window can contain multiple viewports
             *  and happily converts renderWindow coord to viewport coord directly.
             */
            
            Ogre::Viewport* v = display->getCamera()->getViewport();
            Ogre::Vector2 pos = position;
            
            Scene::Element::SPtr element = scene->getElementAtDisplayCoordinate(display, pos, pickPos, rayTo);
            
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
                
                scene->getDebugLines()->addLine(rayTo.getOrigin(), pickPos);
                scene->getDebugLines()->draw();	
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
    
    return VSC::OB::InterfaceResponder::mouseButtonPressed(renderWindow, position, buttonID);
}

bool VSC::OB::SceneController::mouseButtonReleased(Ogre::RenderWindow* renderWindow,
                                                   const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    
    if (mTraceUI)
    {
        std::cout << "VSC::OB::SceneController::mouseButtonReleased : ";
        std::cout << position << " (" << buttonID << ")" << std::endl;
    }
    
    Scene::SPtr scene = this->getScene();
    
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
                
                scene->getDebugLines()->addLine (Ogre::Vector3::ZERO, Ogre::Vector3::ZERO);	
                scene->getDebugLines()->draw();  
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
    
    return VSC::OB::InterfaceResponder::mouseButtonReleased(renderWindow, position, buttonID);
}



// -------------------------------------------------------------------------
bool VSC::OB::SceneController::mouseMoved(Ogre::RenderWindow* renderWindow,
                                          const Ogre::Vector2& position, const Ogre::Vector2& movement)
{
    if (mTraceUI)
    {
        std::cout << "VSC::OB::SceneController::mouseMoved position: ";
        std::cout << position << ", movement: " << movement << "" << std::endl;
    }
    
    Scene::SPtr scene = this->getScene();
    BOOST_ASSERT(scene);
    Display::SPtr display = scene->getApplication()->getDisplayWithRenderWindow(renderWindow);
    
    InterfaceAdapter::SPtr adapter = this->getInterfaceAdapter();
    
    if (adapter->isMouseButtonPressed(OIS::MB_Left))
    {
        if (mPickConstraint)
        {
            if (mTraceUI)
            {
                std::cout << "VSC::OB::SceneController::mouseMoved Left button is pressed and constraint exists" << std::endl;
            }
            
            Ogre::Viewport* v = display->getCamera()->getViewport();
            float normX = position.x / (float) v->getActualWidth();
            float normY = 1.0 - (position.y / (float) v->getActualHeight());
            
            Ogre::Ray rayTo = display->getCamera()->getCameraToViewportRay (normX, normY);
            
            //move the constraint pivot
            OgreBulletDynamics::PointToPointConstraint * p2p =
            static_cast <OgreBulletDynamics::PointToPointConstraint *>(mPickConstraint);
            
            //keep it at the same picking distance
            const Ogre::Vector3 eyePos(display->getCamera()->getDerivedPosition());
            Ogre::Vector3 dir = rayTo.getDirection () * mOldPickingDistance;

            const Ogre::Vector3 newPos (eyePos + dir);
            p2p->setPivotB (newPos);    

            scene->getDebugLines()->addLine (mPickedBody->getWorldPosition (), newPos);
            scene->getDebugLines()->draw();
            
        }
        
        else
        {
            if (mTraceUI) std::cout << "VSC::OB::SceneController::mouseMoved Left button is pressed and constraint does not exists" << std::endl;
        }
        
        return true;
    }

    return VSC::OB::InterfaceResponder::mouseMoved(renderWindow, position, movement);

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
    
    Scene::SPtr scene = this->getScene();
    BOOST_ASSERT(scene);
    Display::SPtr display = scene->getApplication()->getDisplayWithRenderWindow(renderWindow);
    
    InterfaceAdapter::SPtr inputAdapter = this->getInterfaceAdapter();
    
    BOOST_ASSERT_MSG(inputAdapter, "Expected input adapter");
    
    if (!inputAdapter) return false;
    
    OIS::Keyboard::Modifier modifier = inputAdapter->getCurrentModifier();
    VSC::Keyboard::Combination comb(key, modifier);
    
    std::cout << "Actual input is " << comb << std::endl;
    
    BOOST_ASSERT_MSG(this->getOgreKeyBindings(), "Expected key bindings");
    
    const VSC::OB::KeyboardAction::KeySet& actionKeySet = this->getOgreKeyBindings()->getActionsForInput(comb);
    
    BOOST_FOREACH (VSC::OB::KeyboardAction::Key actionKey, actionKeySet) 
    {
        bool handled = true;
        
        switch(actionKey)
        {
                
            case VSC::OB::KeyboardAction::SaveScreenShot:
            {
                if (renderWindow)
                {
                    Ogre::RenderWindow* w = (Ogre::RenderWindow*)renderWindow;
                    w->writeContentsToFile("OgreBulletScreenShot" + StringConverter::toString(count++) + ".png");
                    // baaad
                }
            }
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
                if (display)
                {
                    this->throwDynamicObjectPrimitive(renderWindow, VSC::OB::PrimitiveCube,
                                                      display->getCamera()->getDerivedDirection().normalisedCopy()*mShootSpeed);
                }
                break;
                
            case VSC::OB::KeyboardAction::ShootSphere:
                if (display)
                {
                    this->throwDynamicObjectPrimitive(renderWindow, VSC::OB::PrimitiveSphere,
                                                      display->getCamera()->getDerivedDirection().normalisedCopy()*mShootSpeed);
                }
                break;
                
            case VSC::OB::KeyboardAction::ShootCylinder:
                if (display)
                {
                    this->throwDynamicObjectPrimitive(renderWindow, VSC::OB::PrimitiveCylinder,
                                                      display->getCamera()->getDerivedDirection().normalisedCopy()*mShootSpeed);
                }
                break;
                
            case VSC::OB::KeyboardAction::ShootCone:
                if (display)
                {
                    this->throwDynamicObjectPrimitive(renderWindow, VSC::OB::PrimitiveCone,
                                                      display->getCamera()->getDerivedDirection().normalisedCopy()*mShootSpeed);
                }
                break;
                
            default:
                handled = false;
                break;
        }
        
        if (handled) return true;
    }
    
    return VSC::OB::InterfaceResponder::keyPressed(renderWindow, key);
}

// -------------------------------------------------------------------------
bool VSC::OB::SceneController::keyReleased(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    return VSC::OB::InterfaceResponder::keyReleased(renderWindow, key);
}

bool VSC::OB::SceneController::contextChanged(Ogre::RenderWindow* renderWindow)
{
    Scene::SPtr scene = this->getScene();
    BOOST_ASSERT(scene);
    Display::SPtr display = scene->getApplication()->getDisplayWithRenderWindow(renderWindow);
    
    if (display)
    {
        display->resetCameraAspectRatio();
        return true;
    }
    
    return false;
}

// -------------------------------------------------------------------------
bool VSC::OB::SceneController::throwDynamicObjectPrimitive(Ogre::RenderWindow* renderWindow,
                                                           VSC::OB::PrimitiveType primitiveType,
                                                           const Ogre::Vector3& velocity)
{
    BOOST_ASSERT(renderWindow);
    if (!renderWindow) return false;
    
    const float throwDist = 2.0f;
    
    Scene::SPtr scene = this->getScene();
    BOOST_ASSERT(scene);
    Display::SPtr display = scene->getApplication()->getDisplayWithRenderWindow(renderWindow);
    BOOST_ASSERT(scene == display->getScene());
    if (scene != display->getScene()) return false;
    
    if(!display) return false;
    
    if (scene->checkIfEnoughPlaceToAddObjectForDisplay(display, throwDist) == false)
    {
        // TODO throw exception ?
        return false; // false;
    }
    
    bool handled = true;
    
    BasicSceneElementFactory::SPtr sceneFactory = boost::dynamic_pointer_cast<BasicSceneElementFactory> (scene->getElementFactory());
    
    VSC::OB::DynamicObject::FactoryDescription description;
    
    description.position = display->getCamera()->getDerivedPosition();
    
    VSC::OB::DynamicObject::SPtr object;
    
    switch(primitiveType)
    {
        case VSC::OB::PrimitiveCube:            
            description.name = "Cube";
            description.size = gCubeBodyBounds;
            object = sceneFactory->addPrimitive(VSC::OB::PrimitiveCube, description);
            break;
            
        case VSC::OB::PrimitiveSphere: 
            description.name = "Sphere";
            description.size = gSphereBodyBounds;
            object = sceneFactory->addPrimitive(VSC::OB::PrimitiveSphere, description);
            break;
            
        case VSC::OB::PrimitiveCylinder: 
            description.name = "Cylinder";
            description.size = gCylinderBodyBounds;
            object = sceneFactory->addPrimitive(VSC::OB::PrimitiveCylinder, description);
            break;
            
        case VSC::OB::PrimitiveCone: 
            description.name = "Cone";
            description.size = gConeBodyBounds;
            object = sceneFactory->addPrimitive(VSC::OB::PrimitiveCylinder, description);
            break;
            
        default:
            handled = false;
            break;
    }
    
    if (object)
    {
        object->getRigidBody()->setLinearVelocity(velocity);
    }
    
    if (handled) return true;
    return false;

}

// MARK: - SceneControllerChain

void VSC::OB::SceneControllerChain::removeResponder(InterfaceResponder::SPtr responder)
{
    SceneController::SPtr sceneController = boost::dynamic_pointer_cast<SceneController>(responder);
    BOOST_ASSERT(sceneController);
    if (sceneController)
    {
        sceneController->shutdown();
    }
    InterfaceResponderChain::removeResponder(responder);
}

void VSC::OB::SceneControllerChain::removeAllResponders(void)
{
    BOOST_FOREACH(InterfaceResponder::SPtr responder, this->getResponders())
    {
        SceneController::SPtr sceneController = boost::dynamic_pointer_cast<SceneController>(responder);
        BOOST_ASSERT(sceneController);
        if (sceneController)
        {
            sceneController->shutdown();
        }
    }
    InterfaceResponderChain::removeAllResponders();
}


void VSC::OB::SceneControllerChain::insertResponder(InterfaceResponder::SPtr responder, InterfaceResponders::iterator it)
{
    SceneController::SPtr sceneController = boost::dynamic_pointer_cast<SceneController>(responder);
    BOOST_ASSERT(sceneController);
    if (sceneController) {
        InterfaceResponderChain::insertResponder(responder, it);
    }
}

