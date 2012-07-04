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

using Ogre::Real;
using Ogre::Vector2;

//#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
using namespace OIS;
//#endif //OGRE_VERSION not Eihort

namespace OIS
{
    class Keyboard;
    class Mouse;
};



// -------------------------------------------------------------------------
VSCOgreBulletOISInputListener::VSCOgreBulletOISInputListener(VSCOgreBulletListener * ogreBulletListener, Ogre::RenderWindow *win) :
    VSCOgreBulletInputListener(ogreBulletListener, win)
{
    mWindow = win;
}

