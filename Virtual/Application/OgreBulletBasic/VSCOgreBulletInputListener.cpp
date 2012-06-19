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

using namespace Ogre;
using namespace OIS;

// -------------------------------------------------------------------------
VSCOgreBulletInputListener::VSCOgreBulletInputListener(VSCOgreBulletListener * ogreBulletListener) :
    mButtonLeftPressed (false),
    mButtonMiddlePressed (false),
    mButtonRightPressed (false),
    mListener(ogreBulletListener)
{
    mMouseCursorX = 0.5; 
    mMouseCursorY = 0.5;
}

// -------------------------------------------------------------------------
bool VSCOgreBulletInputListener::keyPressed(const OIS::KeyEvent& e)
{
    mListener->keyPressed(e.key);
    return true;   
}

// -------------------------------------------------------------------------
bool VSCOgreBulletInputListener::keyReleased(const OIS::KeyEvent& e)
{
    mListener->keyReleased(e.key);
    return true;   
}
