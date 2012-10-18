
#include "VSCOBApplication.h"
#include "VSCOBScene.h"

#ifdef __APPLE__
#include "VSCOBOSXApplicationBridge.h"
#else
#error "Unsuported Platform"
#endif

#include "OIS.h"
#include "OgreResourceGroupManager.h"

#include <Ogre/Ogre.h>
#include <Ogre/OgreConfigFile.h>

#include <boost/assert.hpp>
#include <boost/foreach.hpp>


VSC::OB::Application::Application() :
mRoot(0),
Ogre::FrameListener(),
mInitialised(false)
{
#ifdef __APPLE__
    mBridge = Bridge::SPtr(new OSXApplicationBridge);
#endif
}
// -------------------------------------------------------------------------
VSC::OB::Application::~Application()
{ 
    if (mRoot)
        OGRE_DELETE mRoot;
}

VSC::OB::Scene::SPtr VSC::OB::Application::sceneWithName(Ogre::String name)
{
    BOOST_FOREACH (Scene::SPtr scene, mScenes) 
    {
        if (name.compare(scene->getName()) == 0) return scene;
    }
    return Scene::SPtr();
}

bool VSC::OB::Application::init()
{
    BOOST_ASSERT(mBridge);
    if (!mBridge) return false;
    
    mRoot = mBridge->createOgreRoot();
    BOOST_ASSERT(mRoot);
    
    this->setupResources();
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    this->createResourceListener(); // Create any resource listeners (for loading screens)
    this->loadResources();
    
    mRoot->addFrameListener(this);
    
    this->internalInit();
    
    mInitialised = true;
    
    return true;
}

void addScene(Scene::SPtr scene)
{
    BOOST_ASSERT(scene);
    if (!scene) return;
    
    Scenes::const_iterator it = std::find(mScenes.begin(), mScenes.end(), scene);
    BOOST_ASSERT_MSG(it == mScenes.end(), "Scene already added to application");
    if(it == mScenes.end())
    {
        scene->init(shared_from_this());
        mScenes.push_back(scene);
    }
    
}

void removeScene(Scene::SPtr scene)
{
    BOOST_ASSERT(scene);
    if (!scene) return;
    
    Scenes::const_iterator it = std::find(mScenes.begin(), mScenes.end(), scene);
    BOOST_ASSERT_MSG(it != mScenes.end(), "Scene already not in application");
    if(it != mScenes.end())
    {
        scene->shutdown();
        mScenes.erase(scene);
    }
}


bool VSC::OB::Application::frameStarted(const Ogre::FrameEvent& evt)
{
    if (mTraceFrame) std::cout << "VSC::OB::Application frameStarted " << &evt << std::endl;

    BOOST_FOREACH(Scene::SPtr scene, this->getScenes())
    {
        scene->frameStarted(evt.timeSinceLastFrame)
    }
    
    return true;
}


bool VSC::OB::Application::frameEnded(const Ogre::FrameEvent& evt)
{
    
    if (mTraceFrame) std::cout << "VSC::OB::Application::frameEnded" << std::endl;
    
    BOOST_FOREACH(Scene::SPtr scene, this->getScenes())
    {
        scene->frameEnded(evt.timeSinceLastFrame)
    }

    return true; 
}


void VSC::OB::Application::setupResources(void)
{
    Ogre::ResourceGroupManager *rsm = Ogre::ResourceGroupManager::getSingletonPtr();
    
    /*-------------------------------------------------------------------------------------*/
    
    // Load resource paths from config file
    Ogre::ConfigFile cf;
    cf.load(mResourcePath + "resources.cfg");
    
    // Go through all sections & settings in the file
    Ogre::ConfigFile::SectionIterator seci = cf.getSectionIterator();
    
    Ogre::String secName, typeName, archName;
    
    while (seci.hasMoreElements())
    {
        secName = seci.peekNextKey();
        Ogre::ConfigFile::SettingsMultiMap *settings = seci.getNext();
        Ogre::ConfigFile::SettingsMultiMap::iterator i;
        for (i = settings->begin(); i != settings->end(); ++i)
        {
            typeName = i->first;
            archName = i->second;
            try {
                rsm->addResourceLocation(archName, typeName, secName);
            } catch (Ogre::Exception& e) {
                std::cout << "Exception : " << e.getFullDescription() << std::endl;
            }
        }
    }
    
    /*-------------------------------------------------------------------------------------*/
    
	Ogre::StringVector groups = rsm->getResourceGroups();        
	Ogre::FileInfoListPtr finfo =  rsm->findResourceFileInfo ("Bootstrap", "axes.mesh");
    
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
            
            baseName = "/Library/VirtualSoundControl/Ogrebullet/Media";
            rsm->addResourceLocation(baseName, "FileSystem", resName);
			rsm->addResourceLocation(baseName + "/textures", "FileSystem", resName);
			rsm->addResourceLocation(baseName + "/overlays", "FileSystem", resName);
			rsm->addResourceLocation(baseName + "/materials", "FileSystem", resName);
			rsm->addResourceLocation(baseName + "/models", "FileSystem", resName);
			rsm->addResourceLocation(baseName + "/gui", "FileSystem", resName);
		}
	}
}


void VSC::OB::Application::loadResources(void)
{
    Ogre::ResourceGroupManager *rsm = Ogre::ResourceGroupManager::getSingletonPtr();
    
    rsm->initialiseAllResourceGroups(); // is this necessary given what follows ?
    
	Ogre::StringVector groups = rsm->getResourceGroups();      
	for (Ogre::StringVector::iterator it = groups.begin(); it != groups.end(); ++it)
	{
		rsm->initialiseResourceGroup((*it));
	}
}


