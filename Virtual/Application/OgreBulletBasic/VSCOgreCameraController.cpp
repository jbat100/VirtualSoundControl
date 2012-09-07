
#include "VSCOgreCameraController.h"
#include "VSCOgreInputAdapter.h"

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
    if (mTraceUI) std::cout << "VSCOgreCameraController mouseMoved position (" << position << "), movement (" << movement << ")" << std::endl;
    
    //bool handled = true;
    
    if (this->getInputAdapter()->isMouseButtonPressed(OIS::MB_Right))
    {
        mCameraRotX = Ogre::Degree(-movement.x * mMouseSensitivity);
        mCameraRotY = Ogre::Degree(-movement.y * mMouseSensitivity);
        return true;
    }
    
    return VSCOgreInputListener::mouseMoved(position, movement);

}


// -------------------------------------------------------------------------
bool VSCOgreCameraController::keyPressed(OIS::KeyCode key)
{
    
    if (mTraceUI) std::cout << "VSCOgreCameraController got key pressed code: " << key << " (W is " << OIS::KC_W << ")" << std::endl; 
    
    bool handled = true;
    
    switch(key)
    {
            
        case OIS::KC_W:
            mCameraTrans.z -= mCameraSpeed;
            if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
            break;
            
        case OIS::KC_S:
            mCameraTrans.z += mCameraSpeed;
            if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
            break;
            
        case OIS::KC_A:
            mCameraTrans.x -= mCameraSpeed;
            if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
            break;
            
        case OIS::KC_D:
            mCameraTrans.x += mCameraSpeed;
            if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
            break;
            
        default:
            handled =  false;
            break;
    }
    
    if (handled) return true;
    return VSCOgreInputListener::keyPressed(key);
    
}

// -------------------------------------------------------------------------
bool VSCOgreCameraController::keyReleased(OIS::KeyCode key)
{
    if (mTraceUI) std::cout << "VSCOgreCameraController keyReleased : " << key << std::endl;
    
    bool handled = true;
    
    switch(key)
    {

        case OIS::KC_W:
            mCameraTrans.z += mCameraSpeed;
            if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
            break;
            
        case OIS::KC_S:
            mCameraTrans.z -= mCameraSpeed;
            if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
            break;

        case OIS::KC_A:
            mCameraTrans.x += mCameraSpeed;
            if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
            break;
            
        case OIS::KC_D:
            mCameraTrans.x -= mCameraSpeed;
            if (mTraceUI) std::cout << "Camera transform is " << mCameraTrans << std::endl;
            break;
            
        default:
            handled = false;
            break;
    }
    
    if (handled) return true;
    return VSCOgreInputListener::keyReleased(key);
}

bool VSCOgreCameraController::frameStarted(Ogre::Real elapsedTime)
{

    if (mTraceFrame) std::cout << "VSCOgreCameraController frameStarted" << std::endl;
    
    BOOST_ASSERT_MSG(this->getInputAdapter() != 0, "Expected adapter");

    
    if (this->getInputAdapter()->isMouseButtonPressed(OIS::MB_Middle))
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


