/***************************************************************************
This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10
Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)
This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/
#include "VSCOgreBulletApplication.h"
#include "VSCOgreBulletScene.h"
#include "VSCOgreApplicationCocoaSetup.h"
#include "VSCOgreInputAdapter.h"
#include "VSCOgreCameraController.h"

#include "OgreResourceGroupManager.h"

#include <boost/assert.hpp>

// -------------------------------------------------------------------------
VSCOgreBulletApplication::VSCOgreBulletApplication(std::vector<VSCOgreBulletScene*> *bulletScenes) : 
    VSCOgreApplication(),
    Ogre::FrameListener(),
    mBulletScenes(bulletScenes),
    mBulletScene(0)
{
    assert (!mBulletScenes->empty());
}
// -------------------------------------------------------------------------
VSCOgreBulletApplication::~VSCOgreBulletApplication()
{ 

}

bool VSCOgreBulletApplication::setupWithOgreView(void* ogreView)
{
    VSCOgreApplication::setupWithOgreView(ogreView);
    switchScene (*(mBulletScenes->begin()));
    mRoot->addFrameListener(this);
    
    return true;
}

// -------------------------------------------------------------------------
bool VSCOgreBulletApplication::switchScene(VSCOgreBulletScene *newScene)
{
    /*
     *  We need to reset the scene managers and everything...
     */
    
    if (mBulletScene)
    {
        this->getInputAdapter()->removeInputListener(mBulletScene->getCameraController().get());
        this->getInputAdapter()->removeInputListener(mBulletScene);
        mBulletScene->shutdown();
    }
    
    if (newScene)
    {
        newScene->init(mRoot, mWindow, this);
        this->getInputAdapter()->addInputListener(newScene);
        this->getInputAdapter()->addInputListener(newScene->getCameraController().get());
    }
    
    mBulletScene = newScene;
    
    return true;
}
// -------------------------------------------------------------------------
bool VSCOgreBulletApplication::frameStarted(const Ogre::FrameEvent& evt)
{
    if (mTraceFrame) std::cout << "VSCOgreBulletApplication frameStarted " << evt << std::endl;

    std::vector <VSCOgreBulletScene *>::iterator it =  mBulletScenes->begin();
    
    while (it != mBulletScenes->end())
    {
        if ((*(*it)->getBoolActivator()) == true || this->isKeyPressed((*it)->getNextKey()))
        {
            switchScene(*it);
            break;
        }
        ++it;
    }	

    assert (mBulletScene);

    if (!mBulletScene->frameStarted(evt.timeSinceLastFrame))
    {
        mBulletScene->shutdown ();
        return false;
    }
    
    return true;
}

// -------------------------------------------------------------------------
bool VSCOgreBulletApplication::frameEnded(const Ogre::FrameEvent& evt)
{
    assert (mBulletScene);
    if (!mBulletScene->frameEnded(evt.timeSinceLastFrame))
    {
        mBulletScene->shutdown();
        return false;
    }
    return true; 
}

// -------------------------------------------------------------------------
void VSCOgreBulletApplication::setupResources(void)
{
	VSCOgreApplication::setupResources(); 
	Ogre::ResourceGroupManager *rsm = Ogre::ResourceGroupManager::getSingletonPtr();
	Ogre::StringVector groups = rsm->getResourceGroups();        
	Ogre::FileInfoListPtr finfo =  Ogre::ResourceGroupManager::getSingleton().findResourceFileInfo ("Bootstrap", "axes.mesh");
    
	const bool isSDK =  (!finfo->empty()) && 
    Ogre::StringUtil::startsWith (finfo->begin()->archive->getName(), "../../media/packs/ogrecore.zip", true);

	const Ogre::String resName ("OgreBullet");
	{
		if (std::find(groups.begin(), groups.end(), resName) == groups.end())
		{

			rsm->createResourceGroup(resName);
            Ogre::String baseName;
            
/*
 *  Hacky temporary stuff by jonathan (given the path to the executable is not known when using workspaces),
 *  The ogrebullet media folder needs to be copied to "/Library/VirtualSoundControl/Ogrebullet/Media"
 */
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            baseName = "/Library/VirtualSoundControl/Ogrebullet/Media";
            rsm->addResourceLocation (baseName, "FileSystem", resName);
			rsm->addResourceLocation (baseName + "/textures", "FileSystem", resName);
			rsm->addResourceLocation (baseName + "/overlays", "FileSystem", resName);
			rsm->addResourceLocation (baseName + "/materials", "FileSystem", resName);
			rsm->addResourceLocation (baseName + "/models", "FileSystem", resName);
			rsm->addResourceLocation (baseName + "/gui", "FileSystem", resName);
#else
			if (isSDK) baseName = "../../../ogrebullet/";
			else baseName = "../../../../../ogreaddons/ogrebullet/";
            rsm->addResourceLocation (baseName + "Demos/Media", "FileSystem", resName);
			rsm->addResourceLocation (baseName + "Demos/Media/textures", "FileSystem", resName);
			rsm->addResourceLocation (baseName + "Demos/Media/overlays", "FileSystem", resName);
			rsm->addResourceLocation (baseName + "Demos/Media/materials", "FileSystem", resName);
			rsm->addResourceLocation (baseName + "Demos/Media/models", "FileSystem", resName);
			rsm->addResourceLocation (baseName + "Demos/Media/gui", "FileSystem", resName);
#endif
		}
	}
}

// -------------------------------------------------------------------------
void VSCOgreBulletApplication::loadResources(void)
{
	Ogre::ResourceGroupManager *rsm = Ogre::ResourceGroupManager::getSingletonPtr();
	Ogre::StringVector groups = rsm->getResourceGroups();      
	for (Ogre::StringVector::iterator it = groups.begin(); it != groups.end(); ++it)
	{
		rsm->initialiseResourceGroup((*it));
	}
	// Initialise, parse scripts etc
	//ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
}

// -------------------------------------------------------------------------