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

    this->setCamera(this->getSceneManager()->createCamera("Cam"));
    //mCamera->setFOVy(Degree(90));
    this->getCamera()->setNearClipDistance(0.1);
    this->getCamera()->setFarClipDistance(100);
    Viewport *vp = win->addViewport(this->getCamera());
    vp->setBackgroundColour(ColourValue(0,0,0));
    
    // Alter the camera aspect ratio to match the viewport
    this->getCamera()->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    this->getCamera()->setPosition(CameraStart);

    VSC::OB::Scene::init(root, win);

    this->setupLights();
    this->initWorld();
    this->getElementFactory()->addGround();

}

