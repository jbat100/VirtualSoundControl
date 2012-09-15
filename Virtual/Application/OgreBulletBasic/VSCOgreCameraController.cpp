
#include "VSCOgreCameraController.h"
#include "VSCOgreInputAdapter.h"
#include "VSCOgreKeyboardAction.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

VSCOgreCameraController::VSCOgreCameraController() :    
mCameraSpeed (0.1f),
mMouseSensitivity (0.13f),
mCameraTrans (Ogre::Vector3::ZERO),
mCameraRotX (0.0),
mCameraRotY (0.0),
mCamera(0)
{


}

// -------------------------------------------------------------------------
bool VSCOgreCameraController::mouseMoved(const Ogre::Vector2& position, const Ogre::Vector2& movement)
{
    if (mTraceUI) std::cout << "VSCOgreCameraController::mouseMoved position " << position << ", movement " << movement << std::endl;
    
    //bool handled = true;
    
    if (this->getInputAdapter()->isMouseButtonPressed(OIS::MB_Right))
    {
        if (mTraceUI) std::cout << "VSCOgreCameraController::mouseMoved RIGHT BUTTON PRESSED" << std::endl;
        mCameraRotX = Ogre::Degree(-movement.x * mMouseSensitivity);
        mCameraRotY = Ogre::Degree(-movement.y * mMouseSensitivity);
        if (mTraceUI) std::cout << "VSCOgreCameraController::mouseMoved mCameraRotX: " << mCameraRotX << " mCameraRotY:" << mCameraRotY << std::endl;
        return true;
    }
    
    return VSCOgreInputListener::mouseMoved(position, movement);

}


// -------------------------------------------------------------------------
bool VSCOgreCameraController::keyPressed(OIS::KeyCode key)
{
    
    if (mTraceUI) std::cout << "VSCOgreCameraController got key pressed code: " << key << " (W is " << OIS::KC_W << ")" << std::endl; 
    
    bool handled = true;
    
    BOOST_ASSERT_MSG(this->getInputAdapter() != 0, "Expected adapter");
    
    if (this->getInputAdapter() != 0) 
    {
        OIS::Keyboard::Modifier modifier = this->getInputAdapter()->getCurrentModifier();
        VSCKeyboard::Combination comb(key, modifier);
        
        const VSCOgreKeyboardAction::KeySet& actionKeySet = this->getOgreKeyBindings()->getActionsForInput(comb);
        
        BOOST_FOREACH (VSCOgreKeyboardAction::Key actionKey, actionKeySet) 
        {
            switch(actionKey)
            {
                    
                case VSCOgreKeyboardAction::MoveCameraForward:
                    mCameraTrans.z -= mCameraSpeed;
                    if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                case VSCOgreKeyboardAction::MoveCameraBackward:
                    mCameraTrans.z += mCameraSpeed;
                    if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                case VSCOgreKeyboardAction::MoveCameraLeft:
                    mCameraTrans.x -= mCameraSpeed;
                    if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                case VSCOgreKeyboardAction::MoveCameraRight:
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
    
    
    return VSCOgreInputListener::keyPressed(key);
    
}

// -------------------------------------------------------------------------
bool VSCOgreCameraController::keyReleased(OIS::KeyCode key)
{
    if (mTraceUI) std::cout << "VSCOgreCameraController keyReleased : " << key << std::endl;
    
    bool handled = true;
    
    BOOST_ASSERT_MSG(this->getInputAdapter(), "Expected adapter");
    BOOST_ASSERT_MSG(this->getOgreKeyBindings(), "Expected key bindings");
    
    if (this->getInputAdapter() != 0) 
    {
        
        OIS::Keyboard::Modifier modifier = this->getInputAdapter()->getCurrentModifier();
        VSCKeyboard::Combination comb(key, modifier);
        
        const VSCOgreKeyboardAction::KeySet& actionKeySet = this->getOgreKeyBindings()->getActionsForInput(comb);
        
        BOOST_FOREACH (VSCOgreKeyboardAction::Key actionKey, actionKeySet) 
        {
            switch(actionKey)
            {
                    
                case VSCOgreKeyboardAction::MoveCameraForward:
                    mCameraTrans.z += mCameraSpeed;
                    if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                case VSCOgreKeyboardAction::MoveCameraBackward:
                    mCameraTrans.z -= mCameraSpeed;
                    if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                case VSCOgreKeyboardAction::MoveCameraLeft:
                    mCameraTrans.x += mCameraSpeed;
                    if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
                    break;
                    
                case VSCOgreKeyboardAction::MoveCameraRight:
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
    
    return VSCOgreInputListener::keyReleased(key);
}

bool VSCOgreCameraController::frameStarted(Ogre::Real elapsedTime)
{

    if (mTraceFrame) std::cout << "VSCOgreCameraController frameStarted" << std::endl;
    
    BOOST_ASSERT_MSG(this->getInputAdapter() != 0, "Expected adapter");

    if (this->getInputAdapter()->isMouseButtonPressed(OIS::MB_Right))
    {
        if (mTraceUI) std::cout << "VSCOgreCameraController detected middle mouse, yaw: " << mCameraRotX << ", pitch: " << mCameraRotY << std::endl;
        mCamera->yaw(mCameraRotX);
        mCamera->pitch(mCameraRotY);
        mCameraRotX = 0;
        mCameraRotY = 0;
    }

    mCamera->moveRelative(mCameraTrans);

    return true;
}


