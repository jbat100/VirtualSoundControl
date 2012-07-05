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

#include "VSCOgreBulletScene.h"
#include "VSCOgreInputListener.h"

using Ogre::Vector2;
using Ogre::Real;
using Ogre::RenderWindow;

// -------------------------------------------------------------------------
VSCOgreInputListener::VSCOgreInputListener(VSCOgreBulletScene* ogreBulletListener, RenderWindow *window) :
    mWindow(window), mListener(ogreBulletListener)
{
    mMouseMovement = Vector2(0.0, 0.0); 
    mMousePosition = Vector2(0.0, 0.0);
}

bool VSCOgreInputListener::isKeyPressed(OIS::KeyCode key)
{
    return (mCurrentKeys.find(key) != mCurrentKeys.end());
}

bool VSCOgreInputListener::isKeyModifier(OIS::KeyCode key)
{
    return (mModifierKeys.find(key) != mModifierKeys.end());
}

bool VSCOgreInputListener::isMouseButtonPressed(OIS::MouseButtonID buttonid) const
{
    return (mCurrentMouseButtons.find(buttonid) != mCurrentMouseButtons.end());
}

void mouseMoved(const Ogre::Vector2& position, const Ogre::Vector2& movement)
{
    mLastMousePosition = position;
    mLastMouseMovement = movement;
    mBufferedMouseMovement += movement;
}

void mouseEntered(const Ogre::Vector2& position)
{
    
}

void mouseExited(const Ogre::Vector2& position)
{
    
}

void mouseButtonPressed(OIS::MouseButtonID buttonID)
{
    
}

void mouseButtonReleased(OIS::MouseButtonID buttonID)
{
    
}

void keyPressed(OIS::KeyCode key)
{
    
}

void keyReleased(OIS::KeyCode key)
{
    
}


