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
VSCOgreBulletApplication::VSCOgreBulletApplication(std::vector<VSCOgreBulletScene*> bulletScenes) : 
    VSCOgreApplication(),
    Ogre::FrameListener(),
    mBulletScenes(bulletScenes),
    mBulletScene(0)
{
    BOOST_ASSERT_MSG (!mBulletScenes.empty(), "Expected keyboard adapter");
}
// -------------------------------------------------------------------------
VSCOgreBulletApplication::~VSCOgreBulletApplication()
{ 

}

bool VSCOgreBulletApplication::setupWithOgreView(void* ogreView)
{
    //VSCOgreApplication::setupWithOgreView(ogreView);
    
    VSCOgreApplication::setupWithOgreView(ogreView);
    
    BOOST_ASSERT_MSG(mBulletScenes.size() != 0, "Expected keyboard adapter");
    
    switchScene (*(mBulletScenes.begin()));
    
    mRoot->addFrameListener(this);
    
    return true;
}

// -------------------------------------------------------------------------
bool VSCOgreBulletApplication::switchScene(VSCOgreBulletScene *newScene)
{
    
    if (mTraceScene && newScene) std::cout << "VSCOgreBulletApplication switching scene to " << newScene << std::endl;
    if (mTraceScene && !newScene) std::cout << "VSCOgreBulletApplication switching scene to NULL" << std::endl;
    
    /*
     *  We need to reset the scene managers and everything...
     */
    
    BOOST_ASSERT_MSG(this->getKeyboardAdapter() != 0, "Expected keyboard adapter");
    BOOST_ASSERT_MSG(this->getMouseAdapter() != 0, "Expected mouse adapter");
    
    if (mBulletScene)
    {
        if (mTraceScene && newScene) std::cout << "Shutting down old scene..." << std::endl;
        
        BOOST_ASSERT_MSG(mRoot == &Ogre::Root::getSingleton(), "Expected mRoot to be Ogre::Root::getSingleton()");
        
        mRoot->removeFrameListener(mBulletScene->getCameraController());
        
        if (this->getKeyboardAdapter() != 0)
        {
            this->getKeyboardAdapter()->removeInputListener(mBulletScene->getCameraController());
            this->getKeyboardAdapter()->removeInputListener(mBulletScene);
        }
        
        if (this->getMouseAdapter() != 0) 
        {
            this->getMouseAdapter()->removeInputListener(mBulletScene->getCameraController());
            this->getMouseAdapter()->removeInputListener(mBulletScene);
        }
        
        mBulletScene->shutdown();
    }
    
    if (newScene)
    {
        if (mTraceScene && newScene) std::cout << "Initializing new scene..." << std::endl;
        newScene->init(mRoot, mWindow, this);
        
        if (this->getKeyboardAdapter() != 0)
        {
            this->getKeyboardAdapter()->addInputListener(newScene);
            this->getKeyboardAdapter()->addInputListener(newScene->getCameraController());
        }
        
        if (this->getMouseAdapter() != 0) 
        {
            this->getMouseAdapter()->addInputListener(newScene->getCameraController());
            this->getMouseAdapter()->addInputListener(newScene);
        }
        
        BOOST_ASSERT_MSG(mRoot == &Ogre::Root::getSingleton(), "Expected mRoot to be Ogre::Root::getSingleton()");
        
        mRoot->addFrameListener(newScene->getCameraController());
        
    }
    
    else 
    {
        if (mTraceScene && newScene) std::cout << "Switched to null scene..." << std::endl;
    }
    
    mBulletScene = newScene;
    
    return true;
}
// -------------------------------------------------------------------------
bool VSCOgreBulletApplication::frameStarted(const Ogre::FrameEvent& evt)
{
    if (mTraceFrame) std::cout << "VSCOgreBulletApplication frameStarted " << &evt << std::endl;

    std::vector <VSCOgreBulletScene *>::iterator it =  mBulletScenes.begin();
    
    while (it != mBulletScenes.end())
    {
        if ((*(*it)->getBoolActivator()) == true || this->getKeyboardAdapter()->isKeyPressed((*it)->getNextKey()))
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