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
#include "VSCOgreBulletInputListener.h"

// -------------------------------------------------------------------------
VSCOgreBulletInputListener::VSCOgreBulletInputListener(VSCOgreBulletListener* ogreBulletListener) :
    mListener(ogreBulletListener)
{
    mLastMouseMovement = Vector2(0.0, 0.0); 
    mLastMousePosition = Vector2(0.0, 0.0);
}

bool VSCOgreBulletInputListener::isKeyPressed(OIS::KeyCode key)
{
    return (mCurrentKeys.find(key) != mCurrentKeys.end());
}

bool VSCOgreBulletInputListener::isKeyModifier(OIS::KeyCode key)
{
    return (mModifierKeys.find(key) != mModifierKeys.end());
}

bool VSCOgreBulletInputListener::isMouseButtonPressed(OIS::MouseButtonID buttonid) const
{
    return (mCurrentMouseButtons.find(buttonid) != mCurrentMouseButtons.end());
}

// -------------------------------------------------------------------------
bool VSCOgreBulletInputListener::mouseClicked(const OIS::MouseEvent& e)
{
    return true;
}

// -------------------------------------------------------------------------
bool VSCOgreBulletInputListener::mouseEntered(const OIS::MouseEvent& e)
{
    mLastMouseMovement = Vector2(e.state.X.rel, e.state.Y.rel);
    mLastMousePosition = Vector2(Real(e.state.X.abs) / mWindow->getWidth(), Real(e.state.Y.abs) / mWindow->getHeight());
    mListener->mouseEntered(mLastMouseMovement);
    return true;
}

// -------------------------------------------------------------------------
bool VSCOgreBulletInputListener::mouseExited(const OIS::MouseEvent& e)
{
    mLastMouseMovement = Vector2(e.state.X.rel, e.state.Y.rel);
    mLastMousePosition = Vector2(Real(e.state.X.abs) / mWindow->getWidth(), Real(e.state.Y.abs) / mWindow->getHeight());
    mListener->mouseExited(mLastMouseMovement);
    return true;
}

// -------------------------------------------------------------------------
bool VSCOgreBulletInputListener::mousePressed(const OIS::MouseEvent& e, OIS::MouseButtonID buttonid)
{
    mCurrentMouseButtons.insert(buttonid);
    mListener->mouseButtonPressed(buttonid);
    return true;
}

// -------------------------------------------------------------------------
bool VSCOgreBulletInputListener::mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID buttonid)
{
    mCurrentMouseButtons.erase(buttonid);
    mListener->mouseButtonReleased (buttonid);
    return true;   
}

// -------------------------------------------------------------------------
bool VSCOgreBulletInputListener::mouseDragged(const OIS::MouseEvent& e)
{ 
    // This populates the cursor moves or camera rotation variables
    mLastMouseMovement = Vector2(e.state.X.rel, e.state.Y.rel);
    mLastMousePosition = Vector2(Real(e.state.X.abs) / mWindow->getWidth(), Real(e.state.Y.abs) / mWindow->getHeight());
    mListener->mouseDragged(mLastMousePosition, mLastMouseMovement);
    return true;   
}

// -------------------------------------------------------------------------
bool VSCOgreBulletInputListener::mouseMoved(const OIS::MouseEvent& e)
{
    // This populates the cursor moves or camera rotation variables
    mLastMouseMovement = Vector2(e.state.X.rel, e.state.Y.rel);
    mLastMousePosition = Vector2(Real(e.state.X.abs) / mWindow->getWidth(), Real(e.state.Y.abs) / mWindow->getHeight());
    mListener->mouseMoved(mLastMousePosition.x, mLastMousePosition.y, mLastMouseMovement.x, mLastMouseMovement.y);
    return true;    
}

// -------------------------------------------------------------------------
bool VSCOgreBulletInputListener::keyClicked(const OIS::KeyEvent& e)
{
    return true;   
}

// -------------------------------------------------------------------------
bool VSCOgreBulletInputListener::keyPressed(const OIS::KeyEvent& e)
{
    mCurrentKeys.insert(e.key);
    mListener->keyPressed(e.key);
    return true;   
}

// -------------------------------------------------------------------------
bool VSCOgreBulletInputListener::keyReleased(const OIS::KeyEvent& e)
{
    mCurrentKeys.erase(e.key);
    mListener->keyReleased(e.key);
    return true;   
}