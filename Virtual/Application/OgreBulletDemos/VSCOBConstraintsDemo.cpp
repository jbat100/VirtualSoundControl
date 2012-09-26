/***************************************************************************

This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10

Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)



This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/
#include "VSCOBConstraintsDemo.h"

#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "Debug/OgreBulletCollisionsDebugDrawer.h"

#include "VSCOBInputAdapter.h"

using namespace OIS;
using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

// -------------------------------------------------------------------------
const Ogre::Vector3    CameraStart            = Ogre::Vector3(0,-9,1);
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void VSC::OB::ConstraintsDemo::init(Ogre::Root *root, Ogre::RenderWindow *win)
{

    this->setSceneManager(root->createSceneManager(ST_GENERIC));

    this->setCamera(this->getSceneManager()->createCamera("Cam"));
    //mCamera->setFOVy(Degree(90));
    this->getCamera()->setNearClipDistance(0.1);
    this->getCamera()->setFarClipDistance(100);
    Viewport *vp = win->addViewport(this->getCamera());
    vp->setBackgroundColour(ColourValue(0,0,0));
    
    // Alter the camera aspect ratio to match the viewport
    this->getCamera()->setAspectRatio(Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    this->getCamera()->setPosition(CameraStart);
    this->getCamera()->rotate(Ogre::Vector3(1,0,0), Degree(90));
    this->getCamera()->setFixedYawAxis(true, Ogre::Vector3::UNIT_Z);

    VSC::OB::Scene::init(root, win);

    setupLights();
    
    initWorld();

    this->getElementFactory()->addGround();
}

