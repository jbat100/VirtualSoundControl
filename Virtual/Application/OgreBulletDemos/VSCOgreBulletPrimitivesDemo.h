/***************************************************************************
This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10
Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)
This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/

#ifndef _VSC_OGREBULLET_PRIMITIVES_DEMO_H_
#define _VSC_OGREBULLET_PRIMITIVES_DEMO_H_

#include "OgreBulletDynamics.h"
#include "VSCOgreBulletListener.h"

class VSCOgreBulletApplication;

class VSCOgreBulletPrimitivesDemo : public VSCOgreBulletListener 
{
public:
    
    VSCOgreBulletPrimitivesDemo() : VSCOgreBulletListener() {mName = "Primitives Test Scene";};
    virtual ~VSCOgreBulletPrimitivesDemo(){};
    
    void init (Ogre::Root *root, Ogre::RenderWindow *win, VSCOgreBulletApplication *application);
    
    void keyPressed(OIS::KeyCode key);
};

#endif //_VSC_OGREBULLET_PRIMITIVES_DEMO_H_
