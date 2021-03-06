
#include "VSCOBApplication.h"
#include "VSCOBDisplayController.h"
#include "VSCOBInterface.h"
#include "VSCOBKeyBindings.h"
#include "VSCOBMouseBindings.h"

#include "OIS.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

VSC::OB::DisplayController::DisplayController() :    
mCameraSpeed (10.0f),
mMouseSensitivity (0.13f),
mCameraRotating (false),
mCameraRotX (0.0),
mCameraRotY (0.0),
mCameraMovement(CameraMovementNone)
{

}

void VSC::OB::DisplayController::setupWithDisplay(Display::SPtr display)
{
    mDisplay = Display::WPtr(display);
    
    Application::singletonApplication()->getOgreRoot()->addFrameListener(this);
}

void VSC::OB::DisplayController::shutdown(void)
{
    Application::singletonApplication()->getOgreRoot()->removeFrameListener(this);
}

bool VSC::OB::DisplayController::mouseButtonPressed(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& p, OIS::MouseButtonID buttonID)
{
    do {
        
        MouseBindings::SPtr mouseBindings = this->getOBMouseBindings();
        BOOST_ASSERT(mouseBindings);
        if (!mouseBindings) break;
        
        InterfaceAdapter::SPtr adapter = this->getInterfaceAdapter();
        BOOST_ASSERT(adapter);
        if (!adapter) break;
        
        OIS::Keyboard::Modifier modifier = adapter->getCurrentModifier();
        Mouse::Combination combination(buttonID, modifier);
        
        if (mTraceUI)
        {
            std::cout << "VSC::OB::DisplayController::mouseButtonPressed buttonID " << buttonID;
            std::cout << ", modifier " << modifier << std::endl;
        }
        
        const MouseAction::KeySet& actionKeySet = mouseBindings->getActionsForInput(combination);
        
        BOOST_FOREACH(const MouseAction::Key& key, actionKeySet)
        {
            if (mTraceUI) std::cout << "VSC::OB::DisplayController::mouseButtonPressed action: " << key << std::endl;
            
            if (key == MouseAction::CameraMove)
            {
                mCameraRotating = true;
                return true;
            }
        }
        
    } while (0);
    
    return InterfaceResponder::mouseButtonPressed(renderWindow, p, buttonID);
}

bool VSC::OB::DisplayController::mouseButtonReleased(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& p, OIS::MouseButtonID buttonID)
{
    do {
        
        MouseBindings::SPtr mouseBindings = this->getOBMouseBindings();
        BOOST_ASSERT(mouseBindings);
        if (!mouseBindings) break;
        
        InterfaceAdapter::SPtr adapter = this->getInterfaceAdapter();
        BOOST_ASSERT(adapter);
        if (!adapter) break;
        
        /*
         *  If the camera is rotating then we don't care about the modifier, if the mouse button matches camera move action,
         *  then stop rotating
         */
        
        const Mouse::Combination::Set& combinations = mouseBindings->getInputsForAction(MouseAction::CameraMove);
        
        BOOST_FOREACH(const Mouse::Combination& combination, combinations)
        {
            if (combination.button == buttonID)
            {
                mCameraRotating = false;
                return true;
            }
        }
        
    } while (0);
    
    return InterfaceResponder::mouseButtonPressed(renderWindow, p, buttonID);
}

// -------------------------------------------------------------------------
bool VSC::OB::DisplayController::mouseMoved(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, const Ogre::Vector2& movement)
{
    if (mTraceUI) std::cout << "VSC::OB::DisplayController::mouseMoved position " << position << ", movement " << movement << std::endl;
    
    if (mCameraRotating)
    {
        if (mTraceUI) std::cout << "VSC::OB::DisplayController::mouseMoved mCameraRotating == true" << std::endl;
        
        mCameraRotX = Ogre::Degree(-movement.x * mMouseSensitivity);
        mCameraRotY = Ogre::Degree(-movement.y * mMouseSensitivity);
        
        if (mTraceUI)
        {
            std::cout << "VSC::OB::DisplayController::mouseMoved mCameraRotX: " << mCameraRotX;
            std::cout << " mCameraRotY:" << mCameraRotY << std::endl;
        }
        
        return true;
    }
    
    return InterfaceResponder::mouseMoved(renderWindow, position, movement);

}


// -------------------------------------------------------------------------
bool VSC::OB::DisplayController::keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    
    if (mTraceUI) std::cout << "VSC::OB::DisplayController got key pressed code: " << key << " (W is " << OIS::KC_W << ")" << std::endl; 
    
    InterfaceAdapter::SPtr adapter = this->getInterfaceAdapter();
    
    bool handled = true;
    
    BOOST_ASSERT_MSG(adapter, "Expected adapter");
    BOOST_ASSERT_MSG(this->getOBKeyBindings(), "Expected key bindings");
    
    if (adapter && this->getOBKeyBindings())
    {
        OIS::Keyboard::Modifier modifier = adapter->getCurrentModifier();
        VSC::Keyboard::Combination comb(key, modifier);
        
        const VSC::OB::KeyboardAction::KeySet& actionKeySet = this->getOBKeyBindings()->getActionsForInput(comb);
        
        if (mTraceUI) std::cout << "actionKeySet has " << actionKeySet.size() << " elements" << std::endl;
        
        BOOST_FOREACH (VSC::OB::KeyboardAction::Key actionKey, actionKeySet) 
        {
            switch(actionKey)
            {
                    
                case VSC::OB::KeyboardAction::MoveCameraForward:
                    //mCameraTrans.z -= mCameraSpeed;
                    mCameraMovement = (CameraMovement)(mCameraMovement | CameraMovementForward);
                    if (mTraceUI) std::cout << "Camera forward " << std::endl; // << mCameraTrans << std::endl;
                    break;
                    
                case VSC::OB::KeyboardAction::MoveCameraBackward:
                    //mCameraTrans.z += mCameraSpeed;
                    mCameraMovement = (CameraMovement)(mCameraMovement | CameraMovementBackward);
                    if (mTraceUI) std::cout << "Camera backward " << std::endl; // << mCameraTrans << std::endl;
                    break;
                    
                case VSC::OB::KeyboardAction::MoveCameraLeft:
                    //mCameraTrans.x -= mCameraSpeed;
                    mCameraMovement = (CameraMovement)(mCameraMovement | CameraMovementLeft);
                    if (mTraceUI) std::cout << "Camera left " << std::endl; // << mCameraTrans << std::endl;
                    break;
                    
                case VSC::OB::KeyboardAction::MoveCameraRight:
                    //mCameraTrans.x += mCameraSpeed;
                    mCameraMovement = (CameraMovement)(mCameraMovement | CameraMovementRight);
                    if (mTraceUI) std::cout << "Camera right " << std::endl; // << mCameraTrans << std::endl;
                    break;
                    
                default:
                    handled =  false;
                    break;
            }
        }
        
        if (handled) return true;
    }
    
    
    return InterfaceResponder::keyPressed(renderWindow, key);
    
}



// -------------------------------------------------------------------------
bool VSC::OB::DisplayController::keyReleased(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    if (mTraceUI) std::cout << "VSC::OB::DisplayController keyReleased : " << key << std::endl;
    
    InterfaceAdapter::SPtr adapter = this->getInterfaceAdapter();
    
    bool handled = true;
    
    BOOST_ASSERT_MSG(adapter, "Expected adapter");
    BOOST_ASSERT_MSG(this->getOBKeyBindings(), "Expected key bindings");
    
    if (adapter) 
    {
        
        OIS::Keyboard::Modifier modifier = adapter->getCurrentModifier();
        VSC::Keyboard::Combination comb(key, modifier);
        
        const VSC::OB::KeyboardAction::KeySet& actionKeySet = this->getOBKeyBindings()->getActionsForInput(comb);
        
        BOOST_FOREACH (VSC::OB::KeyboardAction::Key actionKey, actionKeySet) 
        {
            switch(actionKey)
            {
                    
                case VSC::OB::KeyboardAction::MoveCameraForward:
                    mCameraMovement = (CameraMovement)(mCameraMovement & (~CameraMovementForward));
                    //if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                case VSC::OB::KeyboardAction::MoveCameraBackward:
                    mCameraMovement = (CameraMovement)(mCameraMovement & (~CameraMovementBackward));
                    //if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                case VSC::OB::KeyboardAction::MoveCameraLeft:
                    mCameraMovement = (CameraMovement)(mCameraMovement & (~CameraMovementLeft));
                    //if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                case VSC::OB::KeyboardAction::MoveCameraRight:
                    mCameraMovement = (CameraMovement)(mCameraMovement & (~CameraMovementRight));
                    //if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                default:
                    handled = false;
                    break;
            }
        }
        
        if (handled) return true;
        
    }
    
    return InterfaceResponder::keyReleased(renderWindow, key);
}

bool VSC::OB::DisplayController::contextChanged(Ogre::RenderWindow* renderWindow)
{
    if (this->getDisplay())
    {
        this->getDisplay()->resetCameraAspectRatio();
    }
    
    return false;
}

//MARK: - Ogre::FrameListener

bool VSC::OB::DisplayController::frameStarted(const Ogre::FrameEvent& evt)
{
    Ogre::Real elapsedTime = evt.timeSinceLastFrame;

    if (mTraceFrame) std::cout << "VSC::OB::DisplayController frameStarted" << std::endl;
    
    InterfaceAdapter::SPtr adapter = this->getInterfaceAdapter();
    
    BOOST_ASSERT_MSG(adapter, "Expected adapter");
    
    Ogre::Camera* camera = this->getDisplay()->getCamera();
    
    if (mCameraRotating)
    {
        if (mTraceUI) std::cout << "VSC::OB::DisplayController detected right mouse, yaw: " << mCameraRotX << ", pitch: " << mCameraRotY << std::endl;
        
        if (camera)
        {
            camera->yaw(mCameraRotX);
            camera->pitch(mCameraRotY);
            mCameraRotX = 0;
            mCameraRotY = 0;
        }
        else
        {
            if (mTraceUI) std::cout << "VSC::OB::DisplayController has no camera " << std::endl;
        }
    }
    
    if (camera)
    {
        Ogre::Vector3 trans = Ogre::Vector3::ZERO;
        
        if (mCameraMovement & CameraMovementForward)    trans += Ogre::Vector3(0, 0, -mCameraSpeed);
        if (mCameraMovement & CameraMovementBackward)   trans += Ogre::Vector3(0, 0, mCameraSpeed);
        if (mCameraMovement & CameraMovementLeft)       trans += Ogre::Vector3(-mCameraSpeed, 0, 0);
        if (mCameraMovement & CameraMovementRight)      trans += Ogre::Vector3(mCameraSpeed, 0, 0);
        
        if (mTraceFrame) std::cout << "VSC::OB::DisplayController::frameStarted trans is " << trans << std::endl;
        
        camera->moveRelative(trans*elapsedTime);
    }
    
    return true;
}

// MARK: - SceneControllerChain

void VSC::OB::DisplayControllerChain::removeResponder(InterfaceResponder::SPtr responder)
{
    DisplayController::SPtr displayController = boost::dynamic_pointer_cast<DisplayController>(responder);
    BOOST_ASSERT(displayController);
    if (displayController)
    {
        displayController->shutdown();
    }
    InterfaceResponderChain::removeResponder(responder);
}

void VSC::OB::DisplayControllerChain::removeAllResponders(void)
{
    BOOST_FOREACH(InterfaceResponder::SPtr responder, this->getResponders())
    {
        DisplayController::SPtr displayController = boost::dynamic_pointer_cast<DisplayController>(responder);
        BOOST_ASSERT(displayController);
        if (displayController)
        {
            displayController->shutdown();
        }
    }
    InterfaceResponderChain::removeAllResponders();
}


void VSC::OB::DisplayControllerChain::insertResponder(InterfaceResponder::SPtr responder, InterfaceResponders::iterator it)
{
    DisplayController::SPtr displayController = boost::dynamic_pointer_cast<DisplayController>(responder);
    BOOST_ASSERT(displayController);
    if (displayController) {
        InterfaceResponderChain::insertResponder(displayController, it);
    }
}


