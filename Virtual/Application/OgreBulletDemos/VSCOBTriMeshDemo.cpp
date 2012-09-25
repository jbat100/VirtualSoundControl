/***************************************************************************

This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10

Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)

This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/
#include "VSCOBTriMeshDemo.h"
#include "VSCOBApplication.h"

#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "Debug/OgreBulletCollisionsDebugDrawer.h"

#include "VSCOBInputAdapter.h"

using namespace OIS;
using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

// -------------------------------------------------------------------------
const Ogre::Vector3 CameraStart  = Ogre::Vector3(0,-9,1);
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void VSC::OB::TriMeshDemo::init(Ogre::Root *root, Ogre::RenderWindow *win, VSC::OB::Application *application)
{

    mSceneMgr = root->createSceneManager(ST_GENERIC);

    mCamera = mSceneMgr->createCamera("Cam");
    mCamera->setFOVy(Degree(90));
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(100);
    Viewport *vp = win->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio( Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    mCamera->setPosition(CameraStart);
    mCamera->rotate(Ogre::Vector3(1,0,0), Degree(90));
    mCamera->setFixedYawAxis(true, Ogre::Vector3::UNIT_Z);

    VSC::OB::Scene::init(root, win);

    setupLights();

    initWorld(Ogre::Vector3(0, 0, -9.81));

    BasicSceneElementFactory::SPtr factory = boost::dynamic_pointer_cast<BasicSceneElementFactory>(this->getElementFactory());
    
    DynamicObject::FactoryDespription description;
    
    description.name = "TriMesh";
    description.position = Ogre::Vector3(0,0,19);
    description.orientation = Ogre::Quaternion::IDENTITY;
    description.bodyFriction = 0.1f;
    description.bodyRestitution = 0.6f;
    
    factory->addTrimesh("RoomComplex.mesh", description);
    
}

void VSC::OB::TriMeshDemo::setupLights()
{
    this->getSceneManager()->setAmbientLight(ColourValue(0.75, 0.75, 0.75));
    
    Ogre::Light* l = 0;
    
    l = this->getSceneManager()->createLight("Sun");
    l->setPosition(CameraStart + Ogre::Vector3(0,0,15));
    l->setType(Light::LT_DIRECTIONAL);
    l->setDirection(-0.40824828, -0.40824828, -0.81649655);
    l->setCastShadows(true);
    l->setSpecularColour(0.5,0.5,0.3);
    this->getLightMap().insert(LightMap::value_type("Sun", l));
    
    l = mSceneMgr->createLight("Spot");
    l->setPosition(CameraStart + Ogre::Vector3(0,0,15));
    l->setType(Light::LT_SPOTLIGHT);
    l->setDirection(-0.40824828, -0.40824828, -0.81649655);
    l->setCastShadows(true);
    l->setSpecularColour(0.5,0.5,0.3);
    this->getLightMap().insert(LightMap::value_type("Sun", l));
    
}

