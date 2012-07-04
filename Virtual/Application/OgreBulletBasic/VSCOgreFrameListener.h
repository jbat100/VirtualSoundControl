/*
-----------------------------------------------------------------------------
This source file is part of OGRE
    (Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2006 Torus Knot Software Ltd
Also see acknowledgements in Readme.html

You may use this sample code for anything you like, it is not covered by the
LGPL like the rest of the engine.
-----------------------------------------------------------------------------
*/
/*
-----------------------------------------------------------------------------
Filename:    ExampleFrameListener.h
Description: Defines an example frame listener which responds to frame events.
This frame listener just moves a specified camera around based on
keyboard and mouse movements.
Mouse:    Freelook
W or Up:  Forward
S or Down:Backward
A:        Step left
D:        Step right
             PgUp:     Move upwards
             PgDown:   Move downwards
             F:        Toggle frame rate stats on/off
			 R:        Render mode
             T:        Cycle texture filtering
                       Bilinear, Trilinear, Anisotropic(8)
             P:        Toggle on/off display of camera position / orientation
-----------------------------------------------------------------------------
*/

#ifndef _VSC_OGRE_FRAME_LISTENER_H_
#define _VSC_OGRE_FRAME_LISTENER_H_

#include <Ogre/Ogre.h>
#include <Ogre/OgreConfigFile.h>
#include <Ogre/OgreException.h>

#include "OIS.h"

class VSCOgreBulletInputListener;

class VSCOgreFrameListener: public Ogre::FrameListener, public Ogre::WindowEventListener
{
    
protected:
    
	virtual void updateStats(void);

public:
    
    VSCOgreFrameListener();
    
#if VSC_ENABLE_OIS_INPUT_SYSTEM
	// Constructor takes a RenderWindow because it uses that to determine input context
	VSCOgreFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, bool bufferedKeys = false, bool bufferedMouse = false, bool bufferedJoy = false);
#else
    VSCOgreFrameListener(Ogre::RenderWindow* win, Ogre::Camera* cam, VSCOgreBulletInputListener* inputListener = 0);
    void setInputListener(VSCOgreBulletInputListener* inputListener) {mInputListener = inputListener;}
    VSCOgreBulletInputListener* getInputListener(void) {return mInputListener;}
#endif
    
    virtual ~VSCOgreFrameListener();

	//Adjust mouse clipping area
	virtual void windowResized(Ogre::RenderWindow* rw);
	//Unattach OIS before window shutdown (very important under Linux)
	virtual void windowClosed(Ogre::RenderWindow* rw);
	virtual bool processUnbufferedKeyInput(const Ogre::FrameEvent& evt);
	virtual bool processUnbufferedMouseInput(const Ogre::FrameEvent& evt);
	virtual void moveCamera();

	virtual void showDebugOverlay(bool show);

	// Override frameRenderingQueued event to process that (don't care about frameEnded)
	bool frameRenderingQueued(const Ogre::FrameEvent& evt);
	bool frameEnded(const Ogre::FrameEvent& evt);

protected:
    
	Ogre::Camera* mCamera;
	Ogre::Vector3 mTranslateVector;
	Ogre::Real mCurrentSpeed;
	Ogre::RenderWindow* mWindow;
	bool mStatsOn;

	std::string mDebugText;
	unsigned int mNumScreenShots;
	float mMoveScale;
	float mSpeedLimit;
    
	Ogre::Degree mRotScale;
	// just to stop toggles flipping too fast
	Ogre::Real mTimeUntilNextToggle ;
	Ogre::Radian mRotX, mRotY;
	Ogre::TextureFilterOptions mFiltering;
	int mAniso;

	int mSceneDetailIndex ;
	Ogre::Real mMoveSpeed;
	Ogre::Degree mRotateSpeed;
	Ogre::Overlay* mDebugOverlay;

#if VSC_ENABLE_OIS_INPUT_SYSTEM
	//OIS Input devices
	OIS::InputManager*  mInputManager;
	OIS::Mouse*         mMouse;
	OIS::Keyboard*      mKeyboard;
	OIS::JoyStick*      mJoy;
#else
    VSCOgreBulletInputListener* mInputListener;
#endif
    
};

#endif//_VSC_OGRE_FRAME_LISTENER_H_
