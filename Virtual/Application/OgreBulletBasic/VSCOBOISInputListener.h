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

/*
InputListener.h
-------------
A basic test framework that minimize code in each test scene listener.
*/
#ifndef _VSC_OGRE_BULLET_OIS_INPUT_LISTENER_H_
#define _VSC_OGRE_BULLET_OIS_INPUT_LISTENER_H_

#include <Ogre/Ogre.h>
#include "OIS.h"
#include "VSCOgreInputListener.h"

class VSCOgreBulletOISInputListener : public VSCOgreInputListener
{
public:

	// Constructor/destructor
    VSCOgreBulletOISInputListener(VSCOgreBulletListener *ogreBulletListener, Ogre::RenderWindow *win);
    virtual ~VSCOgreBulletOISInputListener(){};


};

#endif//_VSC_OGRE_BULLET_OIS_INPUT_LISTENER_H_
