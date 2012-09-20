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
    mHelpKeys.clear();

    // ------------------------
    // Start OgreScene
    mSceneMgr = root->createSceneManager(ST_GENERIC);

    mCamera = mSceneMgr->createCamera("Cam");
    mCamera->setFOVy(Degree(90));
    mCamera->setNearClipDistance(0.1);
    mCamera->setFarClipDistance(100);
    Viewport *vp = win->addViewport(mCamera);
    vp->setBackgroundColour(ColourValue(0,0,0));
    // Alter the camera aspect ratio to match the viewport
    mCamera->setAspectRatio(
        Real(vp->getActualWidth()) / Real(vp->getActualHeight()));
    mCamera->setPosition(CameraStart);
    mCamera->rotate(Ogre::Vector3(1,0,0), Degree(90));
    mCamera->setFixedYawAxis(true, Ogre::Vector3::UNIT_Z);


    VSC::OB::Scene::init(root, win, application);

    // ------------------------
    // add lights
    //setBasicLight();

    // Set ambient light
    mSceneMgr->setAmbientLight(ColourValue(0.75, 0.75, 0.75));

    mSunLight = mSceneMgr->createLight("Sun");
    mSunLight->setPosition(CameraStart + Ogre::Vector3(0,0,15));
    mSunLight->setType(Light::LT_DIRECTIONAL);
    mSunLight->setDirection(-0.40824828, -0.40824828, -0.81649655);
    mSunLight->setCastShadows(true);
    mSunLight->setSpecularColour(0.5,0.5,0.3);

    mLight = mSceneMgr->createLight("Spot");
    mLight->setPosition(CameraStart + Ogre::Vector3(0,0,15));
    mLight->setType(Light::LT_SPOTLIGHT);
    mLight->setDirection(-0.40824828, -0.40824828, -0.81649655);
    mLight->setCastShadows(true);
    mLight->setSpecularColour(0.5,0.5,0.3);
    

    // ------------------------
    // Add the Gui
    setPhysicGUI();

    // ------------------------
    // Start Bullet
    initWorld(Ogre::Vector3 (0, 0, -9.81));

    // ------------------------
    // Add the trimesh
    addStaticTrimesh("sceneMesh",
        "RoomComplex.mesh",
        Ogre::Vector3(0,0,19), 
        Quaternion::IDENTITY,
        0.1f, 
        0.8f);   
}
// -------------------------------------------------------------------------
bool VSC::OB::TriMeshDemo::keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    OIS::Keyboard::Modifier modifier = this->getInputAdapter()->getCurrentModifier();
    VSC::Keyboard::Combination comb(key, modifier);
    
    const VSC::OB::KeyboardAction::KeySet& actionKeySet = this->getOgreKeyBindings()->getActionsForInput(comb);
    
    BOOST_FOREACH (VSC::OB::KeyboardAction::Key actionKey, actionKeySet)
    {
        
        VSC::OB::Scene::throwDynamicObject(actionKey);
        VSC::OB::Scene::dropDynamicObject(actionKey);

        return VSC::OB::Scene::keyPressed(renderWindow, key);
        
    }
    
    return false;
}

