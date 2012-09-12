
#ifndef _VSC_OGRE_CAMERA_CONTROLLER_H_
#define _VSC_OGRE_CAMERA_CONTROLLER_H_

#include "VSCOgreInputListener.h"
#include "VSCOgreKeyboardManager.h"
#include <Ogre/Ogre.h>

class VSCOgreBulletScene;


class VSCOgreCameraController : public VSCOgreInputListener
{
public:

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
    
    virtual bool mouseMoved(const Ogre::Vector2& position, const Ogre::Vector2& movement);
    virtual bool keyPressed(OIS::KeyCode key);
    virtual bool keyReleased(OIS::KeyCode key);
    
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
    
    VSCOgreKeyboardManagerPtr   getKeyboardManager(void) {return mKeyboardManager;}
    void                        setKeyboardManager(VSCOgreKeyboardManagerPtr manager) {mKeyboardManager = manager;}
    
    VSCOgreBulletScene*         getScene(void) const {return mScene;}
    void                        setScene(VSCOgreBulletScene* scene) {}
    
    
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
    
    VSCOgreKeyboardManagerPtr           mKeyboardManager;
    
    const static bool                   mTraceUI = false;
    const static bool                   mTraceFrame = false;
    
    VSCOgreBulletScene*                 mScene;

};

#endif //_VSC_OGRE_CAMERA_CONTROLLER_H_

