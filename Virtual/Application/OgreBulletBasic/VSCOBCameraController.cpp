
#include "VSCOBCameraController.h"
#include "VSCOBInputAdapter.h"
#include "VSCOBKeyboardAction.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

VSC::OB::CameraController::CameraController() :    
mCameraSpeed (0.1f),
mMouseSensitivity (0.13f),
mCameraTrans (Ogre::Vector3::ZERO),
mCameraRotX (0.0),
mCameraRotY (0.0),
mCamera(0)
{


}

bool VSC::OB::CameraController::renderWindowChangedSize(Ogre::RenderWindow* renderWindow)
{
    if (mTraceUI) std::cout << "VSC::OB::CameraController::renderWindowChangedSize" << std::endl;
    return false;
}

// -------------------------------------------------------------------------
bool VSC::OB::CameraController::mouseMoved(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, const Ogre::Vector2& movement)
{
    if (mTraceUI) std::cout << "VSC::OB::CameraController::mouseMoved position " << position << ", movement " << movement << std::endl;
    
    //bool handled = true;
    
    InputAdapter::SPtr adapter = this->getInputAdapter().lock();
    
    BOOST_ASSERT_MSG(adapter, "Expected adapter");
    
    if (adapter && adapter->isMouseButtonPressed(OIS::MB_Right))
    {
        if (mTraceUI) std::cout << "VSC::OB::CameraController::mouseMoved RIGHT BUTTON PRESSED" << std::endl;
        mCameraRotX = Ogre::Degree(-movement.x * mMouseSensitivity);
        mCameraRotY = Ogre::Degree(-movement.y * mMouseSensitivity);
        if (mTraceUI) std::cout << "VSC::OB::CameraController::mouseMoved mCameraRotX: " << mCameraRotX << " mCameraRotY:" << mCameraRotY << std::endl;
        return true;
    }
    
    return VSC::OB::InputListener::mouseMoved(renderWindow, position, movement);

}


// -------------------------------------------------------------------------
bool VSC::OB::CameraController::keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    
    if (mTraceUI) std::cout << "VSC::OB::CameraController got key pressed code: " << key << " (W is " << OIS::KC_W << ")" << std::endl; 
    
    InputAdapter::SPtr adapter = this->getInputAdapter().lock();
    
    bool handled = true;
    
    BOOST_ASSERT_MSG(adapter, "Expected adapter");
    BOOST_ASSERT_MSG(this->getOgreKeyBindings(), "Expected key bindings");
    
    if (adapter && this->getOgreKeyBindings())
    {
        OIS::Keyboard::Modifier modifier = adapter->getCurrentModifier();
        VSC::Keyboard::Combination comb(key, modifier);
        
        const VSC::OB::KeyboardAction::KeySet& actionKeySet = this->getOgreKeyBindings()->getActionsForInput(comb);
        
        if (mTraceUI) std::cout << "actionKeySet has " << actionKeySet.size() << " elements" << std::endl;
        
        BOOST_FOREACH (VSC::OB::KeyboardAction::Key actionKey, actionKeySet) 
        {
            switch(actionKey)
            {
                    
                case VSC::OB::KeyboardAction::MoveCameraForward:
                    mCameraTrans.z -= mCameraSpeed;
                    if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                case VSC::OB::KeyboardAction::MoveCameraBackward:
                    mCameraTrans.z += mCameraSpeed;
                    if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                case VSC::OB::KeyboardAction::MoveCameraLeft:
                    mCameraTrans.x -= mCameraSpeed;
                    if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                case VSC::OB::KeyboardAction::MoveCameraRight:
                    mCameraTrans.x += mCameraSpeed;
                    if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                default:
                    handled =  false;
                    break;
            }
        }
        
        if (handled) return true;
    }
    
    
    return VSC::OB::InputListener::keyPressed(renderWindow, key);
    
}

// -------------------------------------------------------------------------
bool VSC::OB::CameraController::keyReleased(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    if (mTraceUI) std::cout << "VSC::OB::CameraController keyReleased : " << key << std::endl;
    
    InputAdapter::SPtr adapter = this->getInputAdapter().lock();
    
    bool handled = true;
    
    BOOST_ASSERT_MSG(adapter, "Expected adapter");
    BOOST_ASSERT_MSG(this->getOgreKeyBindings(), "Expected key bindings");
    
    if (adapter) 
    {
        
        OIS::Keyboard::Modifier modifier = adapter->getCurrentModifier();
        VSC::Keyboard::Combination comb(key, modifier);
        
        const VSC::OB::KeyboardAction::KeySet& actionKeySet = this->getOgreKeyBindings()->getActionsForInput(comb);
        
        BOOST_FOREACH (VSC::OB::KeyboardAction::Key actionKey, actionKeySet) 
        {
            switch(actionKey)
            {
                    
                case VSC::OB::KeyboardAction::MoveCameraForward:
                    mCameraTrans.z += mCameraSpeed;
                    if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                case VSC::OB::KeyboardAction::MoveCameraBackward:
                    mCameraTrans.z -= mCameraSpeed;
                    if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                case VSC::OB::KeyboardAction::MoveCameraLeft:
                    mCameraTrans.x += mCameraSpeed;
                    if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                case VSC::OB::KeyboardAction::MoveCameraRight:
                    mCameraTrans.x -= mCameraSpeed;
                    if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                default:
                    handled = false;
                    break;
            }
        }
        
        if (handled) return true;
        
    }
    
    return VSC::OB::InputListener::keyReleased(renderWindow, key);
}

bool VSC::OB::CameraController::frameStarted(Ogre::Real elapsedTime)
{

    if (mTraceFrame) std::cout << "VSC::OB::CameraController frameStarted" << std::endl;
    
    InputAdapter::SPtr adapter = this->getInputAdapter().lock();
    
    BOOST_ASSERT_MSG(adapter, "Expected adapter");

    if (adapter && adapter->isMouseButtonPressed(OIS::MB_Right))
    {
        if (mTraceUI) std::cout << "VSC::OB::CameraController detected right mouse, yaw: " << mCameraRotX << ", pitch: " << mCameraRotY << std::endl;
        
        if (mCamera) {
            mCamera->yaw(mCameraRotX);
            mCamera->pitch(mCameraRotY);
            mCameraRotX = 0;
            mCameraRotY = 0;
        }
        
        else {
            if (mTraceUI) std::cout << "VSC::OB::CameraController has no camera " << std::endl;
        }
    }
    
    if (mCamera) {
        mCamera->moveRelative(mCameraTrans);
    }
    
    return true;
}


