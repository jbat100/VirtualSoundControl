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
#include "VSCOgreBulletCocoaInputListener.h"
#include "VSCOgreBulletCocoaInputAdapter.h"
#include "OgreOSXCocoaView.h"


// -------------------------------------------------------------------------
VSCOgreBulletCocoaInputListener::VSCOgreBulletCocoaInputListener(VSCOgreBulletListener * ogreBulletListener, Ogre::RenderWindow *window) :
VSCOgreBulletInputListener(ogreBulletListener, window)
{
    /*
     *  Check that mCocoaInputAdapter or maybe for valid arguments in adapter constructor
     */
}


