/***************************************************************************
This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10
Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)
This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/
/***************************************************************************
File modified for VSC project
 -----------------------------------------------------------------------------*/

#include "VSCOgreBulletListener.h"
#include "VSCOgreBulletOISInputListener.h"

using namespace Ogre;

//#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
using namespace OIS;
//#endif //OGRE_VERSION not Eihort

namespace OIS
{
    class Keyboard;
    class Mouse;
};

/*
#define BULLET_KEY_CODE                             OIS::KeyCode
#define BULLET_KEY_EVENT                            const OIS::KeyEvent&
#define BULLET_MOUSE_EVENT                          const OIS::MouseEvent&
#define BULLET_KC                                   OIS::KC
#define BULLET_LISTENER_IMPLEMENTATION_RETURN       bool
#define BULLET_LISTENER_IMPLEMENTATION_RETURN_CODE  return true;
#define BULLET_GETKEY                               e.key
#define BULLET_BUTTON0_DOWN                         buttonid == OIS::MB_Left
#define BULLET_BUTTON1_DOWN                         buttonid == OIS::MB_Middle
#define BULLET_BUTTON2_DOWN                         buttonid == OIS::MB_Right
#define BULLET_BUTTON0_UP                           buttonid == OIS::MB_Left
#define BULLET_BUTTON1_UP                           buttonid == OIS::MB_Middle
#define BULLET_BUTTON2_UP                           buttonid == OIS::MB_Right
#define BULLET_GETRELX                              e.state.X.rel
#define BULLET_GETRELY                              e.state.Y.rel
#define BULLET_GETX                                 e.state.X.abs
#define e.state.Y.abs                                 e.state.Y.abs
 */


// -------------------------------------------------------------------------
VSCOgreBulletOISInputListener::VSCOgreBulletOISInputListener(VSCOgreBulletListener * ogreBulletListener, Ogre::RenderWindow *win) :
    VSCOgreBulletInputListener(ogreBulletListener),
    mWindow(win)
{
    mMouseCursorX = 0.5; 
    mMouseCursorY = 0.5;
}

// -------------------------------------------------------------------------
bool VSCOgreBulletOISInputListener::mouseClicked(const OIS::MouseEvent& e)
{
    return true;
}

// -------------------------------------------------------------------------
bool VSCOgreBulletOISInputListener::mouseEntered(const OIS::MouseEvent& e)
{
    return true;
}

// -------------------------------------------------------------------------
bool VSCOgreBulletOISInputListener::mouseExited(const OIS::MouseEvent& e)
{
    return true;
}

// -------------------------------------------------------------------------
bool VSCOgreBulletOISInputListener::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID buttonid)
{
    if (buttonid == OIS::MB_Left)
    {
        mButtonLeftPressed = true;
    }
    else if (buttonid == OIS::MB_Middle)
    {
        mButton1Pressed = true;
    }
    else if (buttonid == OIS::MB_Right)
    {
        mButton2Pressed = true;
    }
    
    mListener->mouseButtonPressed(buttonid);
    
    return true;
}

// -------------------------------------------------------------------------
bool VSCOgreBulletOISInputListener::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID buttonid)
{
    if (buttonid == OIS::MB_Left)
    {
        mButtonLeftPressed = false;
    }
    if (buttonid == OIS::MB_Middle)
    {
        mButton1Pressed = false;
    }
    if (buttonid == OIS::MB_Right)
    {
        mButton2Pressed = false;
    }
    
    mListener->mouseButtonReleased (buttonid);
    
    return true;   
}

// -------------------------------------------------------------------------
bool VSCOgreBulletOISInputListener::mouseDragged(const OIS::MouseEvent& e)
{ 
    // This populates the cursor moves or camera rotation variables
    mRelX = e.state.X.rel;
    mRelY = e.state.Y.rel;
    mMouseCursorX = Real(e.state.X.abs) / mWindow->getWidth ();
    mMouseCursorY = Real(e.state.Y.abs) / mWindow->getHeight ();
    mListener->mouseMoved ();
    return true;   
}

// -------------------------------------------------------------------------
bool VSCOgreBulletOISInputListener::mouseMoved(const OIS::MouseEvent& e)
{
    // This populates the cursor moves or camera rotation variables
    mRelX = e.state.X.rel;
    mRelY = e.state.Y.rel;
    mMouseCursorX = Real(e.state.X.abs) / mWindow->getWidth ();
    mMouseCursorY = Real(e.state.Y.abs) / mWindow->getHeight ();
    mListener->mouseMoved ();
    
    return true;    
}

// -------------------------------------------------------------------------
bool VSCOgreBulletOISInputListener::keyClicked(const OIS::KeyEvent& e)
{
    return true;   
}

// -------------------------------------------------------------------------
bool VSCOgreBulletOISInputListener::keyPressed(const OIS::KeyEvent& e)
{
    mListener->keyPressed(e.key);
    return true;   
}

// -------------------------------------------------------------------------
bool VSCOgreBulletOISInputListener::keyReleased(const OIS::KeyEvent& e)
{
    mListener->keyReleased(e.key);
    return true;   
}
