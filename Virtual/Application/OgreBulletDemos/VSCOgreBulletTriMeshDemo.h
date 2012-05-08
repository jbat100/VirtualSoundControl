/***************************************************************************
This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10
Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)
This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/

#ifndef _VSC_OGRE_BULLET_TRIMESH_DEMO_H_
#define _VSC_OGRE_BULLET_TRIMESH_DEMO_H_

#include "OgreBulletDynamics.h"

#include "VSCOgreBulletListener.h"
class VSCOgreBulletApplication;

// -------------------------------------------------------------------------
class VSCOgreBulletTriMeshDemo : public VSCOgreBulletListener 
{
public:

    Trimesh_Demo() : VSCOgreBulletListener() {mName = "Trimesh Static Scene test";};
    virtual ~Trimesh_Demo(){};

    void init(Ogre::Root *root, Ogre::RenderWindow *win, VSCOgreBulletApplication *application);

    void keyPressed(BULLET_KEY_CODE key);

};


#endif //_VSC_OGRE_BULLET_TRIMESH_DEMO_H_
