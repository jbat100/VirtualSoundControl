/***************************************************************************
This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10
Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)
This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/

#include "VSCOBPrimitivesDemo.h"

#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "Debug/OgreBulletCollisionsDebugDrawer.h"

#include "VSCOBInputAdapter.h"

using namespace OIS;
using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

// -------------------------------------------------------------------------
const Ogre::Vector3    CameraStart            = Ogre::Vector3(13,4.5,0);
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void VSC::OB::PrimitivesDemo::init(Ogre::Root *root, Ogre::RenderWindow *win)
{

    mSceneMgr = root->createSceneManager(ST_GENERIC);

    mCamera = mSceneMgr->createCamera("Cam");
    //mCamera->setFOVy(Degree(90));
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(100);
    Viewport *vp = win->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));

    mCamera->setPosition(CameraStart);
    
    //mCamera->lookAt(0, 0.5, 0);
    //mCamera->rotate(Ogre::Vector3(1,0,0), Degree(90));
    //mCamera->setFixedYawAxis(true, Ogre::Vector3::UNIT_Z);

    VSC::OB::Scene::init(root, win, application);

    setupLights();

    initWorld();

    this->getElementFactory()->addGround();

}

