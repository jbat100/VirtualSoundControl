/***************************************************************************
This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10
Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)
This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/
#include "VSCOgreBulletApplication.h"
#include "VSCOgreBulletListener.h"
#include "VSCOgreApplicationCocoaSetup.h"

#include "OgreResourceGroupManager.h"

#if !(OGRE_VERSION <  ((1 << 16) | (3 << 8) | 0))
    //using namespace OIS;
#endif //OGRE_VERSION not Eihort

#include <boost/assert.hpp>

//using namespace OgreBulletDynamics;
//using namespace OgreBulletCollisions;
//using namespace OgreBulletLoader;

// -------------------------------------------------------------------------
VSCOgreBulletApplication::VSCOgreBulletApplication(std::vector <VSCOgreBulletListener *> *bulletListeners) : 
    VSCOgreApplication(),
    Ogre::FrameListener(),
#if VSC_ENABLE_OIS_INPUT_SYSTEM
    mInputSystem(0),
    mInput(0),
#endif
    mBulletListener(0)
{
    mBulletListeners = bulletListeners;
    assert (!mBulletListeners->empty());
}
// -------------------------------------------------------------------------
VSCOgreBulletApplication::~VSCOgreBulletApplication()
{ 
#if VSC_ENABLE_OIS_INPUT_SYSTEM
    if (mInputSystem || mInput)
    {
            mInputSystem->destroyInputObject(mInput);
            mInputSystem->destroyInputObject(mMouse);
            OIS::InputManager::destroyInputSystem(mInputSystem);
    }
#endif 
}
// -------------------------------------------------------------------------
bool VSCOgreBulletApplication::switchListener(VSCOgreBulletListener *newListener)
{

#ifdef VSC_ENABLE_OIS_INPUT_SYSTEM
    if (mBulletListener)
    {
        mInput->setEventCallback (0);
        mMouse->setEventCallback (0);
        mBulletListener->shutdown ();
    }
    newListener->init (mRoot, mWindow, this);
    mInput->setEventCallback (newListener->getInputListener());
    mMouse->setEventCallback (newListener->getInputListener());
#endif //VSC_ENABLE_OIS_INPUT_SYSTEM

    mBulletListener = newListener;

    return true;
}
// -------------------------------------------------------------------------
bool VSCOgreBulletApplication::frameStarted(const Ogre::FrameEvent& evt)
{

#ifdef VSC_ENABLE_OIS_INPUT_SYSTEM
    mMouse->capture();
    mInput->capture();
#endif

    std::vector <VSCOgreBulletListener *>::iterator it =  mBulletListeners->begin();
    while (it != mBulletListeners->end())
    {
#ifdef VSC_ENABLE_OIS_INPUT_SYSTEM
        if ((*(*it)->getBoolActivator()) == true || mInput->isKeyDown ((*it)->getNextKey ()))
        {
            //if ((*it) !=  mBulletListener)
            {
                switchListener(*it);
            }
            break;
        }
        ++it;
#endif
    }	

    assert (mBulletListener);

    if (!mBulletListener->frameStarted(evt.timeSinceLastFrame))
    {
        mBulletListener->shutdown ();
        return false;
    }
    return true;
}

// -------------------------------------------------------------------------
bool VSCOgreBulletApplication::frameEnded(const Ogre::FrameEvent& evt)
{
    assert (mBulletListener);
    // we're running a scene, tell it that a frame's started 
    if (!mBulletListener->frameEnded(evt.timeSinceLastFrame))
    {
        mBulletListener->shutdown();
        return false;
    }
    return true; 
}

// -------------------------------------------------------------------------
void VSCOgreBulletApplication::createFrameListener(void)
{
    mFrameListener = 0;

#if VSC_ENABLE_OIS_INPUT_SYSTEM
    size_t windowHnd = 0;
    std::ostringstream windowHndStr;
	OIS::ParamList pl;

    #if defined OIS_WIN32_PLATFORM
    mWindow->getCustomAttribute("WINDOW", &windowHnd);
    #elif defined OIS_LINUX_PLATFORM
    mWindow->getCustomAttribute( "WINDOW", &windowHnd );    
    #else // do it for apple also
    mWindow->getCustomAttribute( "WINDOW", &windowHnd ); 
    #endif    
    windowHndStr << (size_t) windowHnd;
    pl.insert( std::make_pair( std::string( "WINDOW" ), windowHndStr.str() ) );

    unsigned int width, height, depth;
    int left, top;
    mWindow->getMetrics(width, height, depth, left, top);
    
    mInputSystem  = OIS::InputManager::createInputSystem( pl );
    mInput = static_cast<OIS::Keyboard*>(mInputSystem->createInputObject( OIS::OISKeyboard, true ));
    mMouse = static_cast<OIS::Mouse*>(mInputSystem->createInputObject( OIS::OISMouse, true ));
    const OIS::MouseState &ms = mMouse->getMouseState();
    ms.width = width;
    ms.height = height;
#endif

    switchListener (*(mBulletListeners->begin()));
    mRoot->addFrameListener(this);

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
			if (isSDK)
			{
				baseName = "../../../ogrebullet/";
			}
			else
			{
				baseName = "../../../../../ogreaddons/ogrebullet/";
			}
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