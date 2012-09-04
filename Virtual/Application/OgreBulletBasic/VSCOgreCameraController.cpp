
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
void VSCOgreCameraController::mouseMoved(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position, const Ogre::Vector2& movement)
{
    if (mTraceUI) std::cout << "VSCOgreCameraController mouseMoved position (" << position << "), movement (" << movement << ")" << std::endl;
    
    if (this->getMouseAdapter()->isMouseButtonPressed(OIS::MB_Right))
    {
        mCameraRotX = Ogre::Degree(-movement.x * mMouseSensitivity);
        mCameraRotY = Ogre::Degree(-movement.y * mMouseSensitivity);
    }

}


// -------------------------------------------------------------------------
void VSCOgreCameraController::keyPressed(VSCOgreInputAdapter* adapter, OIS::KeyCode key)
{
    
    if (mTraceUI) std::cout << "VSCOgreCameraController got key pressed code: " << key << " (W is " << OIS::KC_W << ")" << std::endl; 
    
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
            break;
    }
}

// -------------------------------------------------------------------------
void VSCOgreCameraController::keyReleased(VSCOgreInputAdapter* adapter, OIS::KeyCode key)
{
    if (mTraceUI) std::cout << "VSCOgreCameraController keyReleased : " << key << std::endl;
    
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
            break;
    }
}

bool VSCOgreCameraController::frameStarted(Ogre::Real elapsedTime)
{

    if (mTraceUI) std::cout << "VSCOgreCameraController frameStarted" << std::endl;
    
    BOOST_ASSERT_MSG(this->getKeyboardAdapter() != 0, "Expected keyboard adapter");
    BOOST_ASSERT_MSG(this->getMouseAdapter() != 0, "Expected mouse adapter");
    
    if (this->getMouseAdapter()->isMouseButtonPressed(OIS::MB_Middle))
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


