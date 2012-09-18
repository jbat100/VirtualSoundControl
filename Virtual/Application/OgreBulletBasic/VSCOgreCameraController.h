
#ifndef _VSC_OGRE_CAMERA_CONTROLLER_H_
#define _VSC_OGRE_CAMERA_CONTROLLER_H_

#include "VSCOgreInputListener.h"
#include "VSCOgreKeyBindings.h"
#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>

class VSCOgreCameraController : public VSCOgreInputListener, public VSCOgreKeyBound
{
    
public:
    
    typedef boost::shared_ptr<VSCOgreCameraController> SPtr;

	/**
     *  Constructor/Destructor
     */
    
    VSCOgreCameraController();

    /**
     *  Ogre Frame Listener Events Forwarded by scene or viewport
     */
    
    bool frameStarted(Ogre::Real elapsedTime);

    /**
     *  Input Listener Overrides, only need to override these
     *  These can be overriden further to provide special camera control...
     */
    
    virtual bool mouseMoved(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, const Ogre::Vector2& movement);
    virtual bool keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key);
    virtual bool keyReleased(Ogre::RenderWindow* renderWindow, OIS::KeyCode key);
    
    virtual bool renderWindowChangedSize(Ogre::RenderWindow* renderWindow);
    
    /**
     *  Ogre Setters/Getters
     */
    
    Ogre::Camera*   getCamera(void) {return mCamera;}
    void            setCamera(Ogre::Camera* cam) {mCamera = cam;}
    
    /**
     *  Other Setters/Getters
     */
    
    float   getMouseSensitivity(void) {return mMouseSensitivity;}
    void    setMouseSensitivity(float sens) {mMouseSensitivity = sens;}
    float   getCameraSpeed(void) {return mCameraSpeed;}
    void    setCameraSpeed(float speed) {mCameraSpeed = speed;}
    
private:

    /** 
     *  Consider using a boost::weak_ptr for the camera, HOWEVER, note that 
     *  it should be destructed using the scene manager that created it which
     *  makes things more complicated.
     */
    
    Ogre::Camera                        *mCamera;
    
    Ogre::Radian                        mCameraRotX;
    Ogre::Radian                        mCameraRotY;
    Ogre::Vector3                       mCameraTrans;
    
    float                               mMouseSensitivity;
    float                               mCameraSpeed;
    
    const static bool                   mTraceUI = true;
    const static bool                   mTraceFrame = false;
    
};

#endif //_VSC_OGRE_CAMERA_CONTROLLER_H_

