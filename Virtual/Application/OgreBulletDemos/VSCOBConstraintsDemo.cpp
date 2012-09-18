/***************************************************************************

This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10

Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)



This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/
#include "VSCOgreBulletConstraintsDemo.h"

#include "OgreBulletDynamicsWorld.h"
#include "OgreBulletDynamicsRigidBody.h"
#include "Debug/OgreBulletCollisionsDebugDrawer.h"

#include "VSCOgreInputAdapter.h"

using namespace OIS;
using namespace Ogre;
using namespace OgreBulletCollisions;
using namespace OgreBulletDynamics;

// -------------------------------------------------------------------------
const Ogre::Vector3    CameraStart            = Ogre::Vector3(0,-9,1);
// -------------------------------------------------------------------------


// -------------------------------------------------------------------------
void VSC::OB::ConstraintsDemo::init(Ogre::Root *root, Ogre::RenderWindow *win, VSC::OB::Application *application)
{
    mHelpKeys.clear();
    mHelpKeys.push_back (BASIC_HELP_INFO0);
    mHelpKeys.push_back (BASIC_HELP_INFO1);
    mHelpKeys.push_back (BASIC_HELP_INFO2);
    mHelpKeys.push_back (BASIC_HELP_INFO3);
    mHelpKeys.push_back (BASIC_HELP_INFO4);
    mHelpKeys.push_back (BASIC_HELP_INFO5);
    mHelpKeys.push_back (BASIC_HELP_INFO6);
    // ------------------------
    // Start OgreScene
    mSceneMgr = root->createSceneManager(ST_GENERIC);

    mCamera = mSceneMgr->createCamera("Cam");
    //mCamera->setFOVy(Degree(90));
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
    setBasicLight();
    
    // ------------------------
    // Add the Gui
    setPhysicGUI();
    // ------------------------
    // Start Bullet
    initWorld();

    // ------------------------
    // Add the ground
    addGround();
}
// -------------------------------------------------------------------------
bool VSC::OB::ConstraintsDemo::keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    bool handled = false;
    
    OIS::Keyboard::Modifier modifier = this->getInputAdapter()->getCurrentModifier();
    VSC::Keyboard::Combination comb(key, modifier);
    
    const VSC::OB::KeyboardAction::KeySet& actionKeySet = this->getOgreKeyBindings()->getActionsForInput(comb);
    
    BOOST_FOREACH (VSC::OB::KeyboardAction::Key actionKey, actionKeySet)
    {
        if (!handled) handled = VSC::OB::Scene::throwDynamicObject(actionKey);
        if (!handled) handled = VSC::OB::Scene::dropDynamicObject(actionKey);
    }
    
    if (handled) return true;
    
    return VSC::OB::Scene::keyPressed(renderWindow, key);
}

