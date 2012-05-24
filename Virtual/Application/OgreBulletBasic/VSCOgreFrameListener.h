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

//Use this define to signify OIS will be used as a DLL
//(so that dll import/export macros are in effect)
#define OIS_DYNAMIC_LIB

// NOTICE: You may need to change this line to <OIS.h> if you build OIS from source.
#include "OIS.h"

using namespace Ogre;

class VSCOgreFrameListener: public FrameListener, public WindowEventListener
{
    
protected:
    
	virtual void updateStats(void);

public:
    
    VSCOgreFrameListener();
    
	// Constructor takes a RenderWindow because it uses that to determine input context
	VSCOgreFrameListener(RenderWindow* win, Camera* cam, 
                         bool bufferedKeys = false, bool bufferedMouse = false, bool bufferedJoy = false);
    
    virtual ~VSCOgreFrameListener();

	//Adjust mouse clipping area
	virtual void windowResized(RenderWindow* rw);

	//Unattach OIS before window shutdown (very important under Linux)
	virtual void windowClosed(RenderWindow* rw);

	virtual bool processUnbufferedKeyInput(const FrameEvent& evt);

	virtual bool processUnbufferedMouseInput(const FrameEvent& evt);

	virtual void moveCamera();

	virtual void showDebugOverlay(bool show);

	// Override frameRenderingQueued event to process that (don't care about frameEnded)
	bool frameRenderingQueued(const FrameEvent& evt);

	bool frameEnded(const FrameEvent& evt);

protected:
    
	Camera* mCamera;

	Vector3 mTranslateVector;
	Real mCurrentSpeed;
	RenderWindow* mWindow;
	bool mStatsOn;

	std::string mDebugText;

	unsigned int mNumScreenShots;
	float mMoveScale;
	float mSpeedLimit;
	Degree mRotScale;
	// just to stop toggles flipping too fast
	Real mTimeUntilNextToggle ;
	Radian mRotX, mRotY;
	TextureFilterOptions mFiltering;
	int mAniso;

	int mSceneDetailIndex ;
	Real mMoveSpeed;
	Degree mRotateSpeed;
	Overlay* mDebugOverlay;

	//OIS Input devices
	OIS::InputManager* mInputManager;
	OIS::Mouse*    mMouse;
	OIS::Keyboard* mKeyboard;
	OIS::JoyStick* mJoy;
    
};

#endif//_VSC_OGRE_FRAME_LISTENER_H_
