
#include "OIS.h"

#include "VSCOBApplication.h"
#include "VSCOBScene.h"
#include "VSCOBInputAdapter.h"
#include "VSCOBCameraController.h"
#include "VSCOBSceneController.h"

#include "VSCUI.h"

#include "OgreResourceGroupManager.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>



VSC::OB::Application::Application(const std::vector<Scene::SPtr>& bulletScenes) :
mRoot(0),
Ogre::FrameListener(),
mScenes(bulletScenes)
{
    
    mKeyboardManager = VSC::OB::KeyboardManager::SPtr(new VSC::OB::KeyboardManager());
    this->setOgreKeyBindings(mKeyboardManager->generateDefaultBindings());
    
    mSceneController = SceneController::SPtr(new SceneController());
    mSceneController->setOgreKeyBindings(this->getOgreKeyBindings());
    
    mCameraController = CameraController::SPtr(new CameraController());
    mCameraController->setOgreKeyBindings(this->getOgreKeyBindings());
    
    this->setNextInputListener(SceneController::WPtr(mSceneController));
    mSceneController->setNextInputListener(CameraController::WPtr(mCameraController));
    
    this->setCollisionMapper(IM::CollisionMapper::SPtr(new IM::CollisionMapper));
    
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
    this->setupResources();
    
    Ogre::TextureManager::getSingleton().setDefaultNumMipmaps(5);
    
    this->createResourceListener(); // Create any resource listeners (for loading screens)
    
    this->loadResources();
    
    mRoot->addFrameListener(this);
    
    return true;
}



// -------------------------------------------------------------------------
bool VSC::OB::Application::switchToScene(Scene::SPtr newScene)
{
    
    if (mTraceScene && newScene) std::cout << "VSC::OB::Application switching scene to " << newScene << std::endl;
    if (mTraceScene && !newScene) std::cout << "VSC::OB::Application switching scene to NULL" << std::endl;
    
    /*
     *  We need to reset the scene managers and everything...
     */
    
    InputAdapter::SPtr adapter = this->getInputAdapter().lock();
    
    BOOST_ASSERT_MSG(adapter, "Expected  adapter");
    
    if (mActiveScene)
    {
        if (mTraceScene && newScene) std::cout << "Shutting down old scene..." << std::endl;
        mActiveScene->getCollisionDetector()->removeListener(this->getCollisionMapper());
        mSceneController->shutdown();
        mActiveScene->shutdown();
    }
    
    if (newScene)
    {
        if (mTraceScene && newScene) std::cout << "Initializing new scene..." << std::endl;
        newScene->init(mRoot, mWindow);
        this->getSceneController()->setScene(Scene::WPtr(newScene));
        this->getCameraController()->setCamera(newScene->getCamera());
        newScene->getCollisionDetector()->addListener(this->getCollisionMapper());
    }
    
    else 
    {
        if (mTraceScene && newScene) std::cout << "Switched to null scene..." << std::endl;
    }
    
    mActiveScene = newScene;
    
    return true;
}


bool VSC::OB::Application::frameStarted(const Ogre::FrameEvent& evt)
{
    if (mTraceFrame) std::cout << "VSC::OB::Application frameStarted " << &evt << std::endl;

    //BOOST_ASSERT_MSG (mActiveScene, "Expected mActiveScene");
    
    if (mActiveScene && !mActiveScene->frameStarted(evt.timeSinceLastFrame))
    {
        mActiveScene->shutdown();
        return false;
    }
    
    this->getCameraController()->frameStarted(evt.timeSinceLastFrame);
    
    return true;
}


bool VSC::OB::Application::frameEnded(const Ogre::FrameEvent& evt)
{
    
    if (mTraceFrame) std::cout << "VSC::OB::Application::frameEnded" << std::endl;
    
    if (mActiveScene && !mActiveScene->frameEnded(evt.timeSinceLastFrame))
    {
        mActiveScene->shutdown();
        return false;
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
                Ogre::ResourceGroupManager::getSingleton().addResourceLocation(archName, typeName, secName);
            } catch (Ogre::Exception& e) {
                std::cout << "Exception : " << e.getFullDescription() << std::endl;
            }
        }
    }
    
    /*-------------------------------------------------------------------------------------*/
    
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

// -------------------------------------------------------------------------
void VSC::OB::Application::loadResources(void)
{
    ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
    
	Ogre::ResourceGroupManager *rsm = Ogre::ResourceGroupManager::getSingletonPtr();
	Ogre::StringVector groups = rsm->getResourceGroups();      
	for (Ogre::StringVector::iterator it = groups.begin(); it != groups.end(); ++it)
	{
		rsm->initialiseResourceGroup((*it));
	}
}

// -------------------------------------------------------------------------

bool VSC::OB::Application::keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    if (mTraceUI) std::cout << "VSC::OB::Application::keyPressed " << key << std::endl;
    
    return VSC::OB::InputListener::keyPressed(renderWindow, key);
}
