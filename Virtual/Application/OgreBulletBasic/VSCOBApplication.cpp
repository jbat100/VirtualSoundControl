    
#include "VSCOBApplication.h"
#include "VSCOBScene.h"
#include "VSCOBOSXApplicationSetup.h"
#include "VSCOBInputAdapter.h"
#include "VSCOBCameraController.h"
#include "VSCOBSceneController.h"

#include "OgreResourceGroupManager.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

// -------------------------------------------------------------------------
VSC::OB::Application::Application(std::vector<Scene::SPtr> bulletScenes) :
    ApplicationBase(),
    Ogre::FrameListener(),
    mBulletScenes(bulletScenes),
    mBulletScene(Scene::SPtr())
{
    BOOST_ASSERT_MSG (!mBulletScenes.empty(), "Expected keyboard adapter");
    
    mSceneController = SceneController::SPtr(new SceneController());
    
    mSceneController->setOgreKeyBindings(this->getKeyboardManager()->generateDefaultBindings());
    
    mCameraController = CameraController::SPtr(new CameraController());
    
    this->setNextInputListener(SceneController::WPtr(mSceneController));
    mSceneController->setNextInputListener(CameraController::WPtr(mCameraController));
}
// -------------------------------------------------------------------------
VSC::OB::Application::~Application()
{ 

}

VSC::OB::Scene::SPtr VSC::OB::Application::sceneWithName(Ogre::String name)
{
    BOOST_FOREACH (Scene::SPtr scene, mBulletScenes) 
    {
        if (name.compare(scene->getName()) == 0) return scene;
    }
    return Scene::SPtr();
}

bool VSC::OB::Application::setupWithOgreView(void* ogreView)
{
    //VSC::OB::ApplicationBase::setupWithOgreView(ogreView);
    
    VSC::OB::ApplicationBase::setupWithOgreView(ogreView);
    
    BOOST_ASSERT_MSG(mBulletScenes.size() != 0, "Expected keyboard adapter");
    
    this->switchToScene(*mBulletScenes.begin());
    
    BOOST_ASSERT_MSG(mRoot == &Ogre::Root::getSingleton(), "Expected mRoot to be Ogre::Root::getSingleton()");
    
    mRoot->addFrameListener(this);
    
    return true;
}

std::vector<Ogre::String> VSC::OB::Application::getSceneNames(void)
{
    std::vector<Ogre::String> sceneNames;
    
    BOOST_FOREACH (VSC::OB::Scene::SPtr scene, mBulletScenes) 
    {
        sceneNames.push_back(scene->getName());
    }
    
    return sceneNames;
}

bool VSC::OB::Application::switchToSceneWithName(Ogre::String sceneName)
{
    VSC::OB::Scene::SPtr scene = this->sceneWithName(sceneName);
    
    if (scene) 
    {
        return this->switchToScene(scene);
    }
    
    return false;
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
    
    if (mBulletScene)
    {
        if (mTraceScene && newScene) std::cout << "Shutting down old scene..." << std::endl;
        mSceneController->shutdown();
        mBulletScene->shutdown();
    }
    
    if (newScene)
    {
        if (mTraceScene && newScene) std::cout << "Initializing new scene..." << std::endl;
        newScene->init(mRoot, mWindow);
        this->getSceneController()->setScene(Scene::WPtr(newScene));
        this->getCameraController()->setCamera(newScene->getCamera());
    }
    
    else 
    {
        if (mTraceScene && newScene) std::cout << "Switched to null scene..." << std::endl;
    }
    
    mBulletScene = newScene;
    
    return true;
}
// -------------------------------------------------------------------------
bool VSC::OB::Application::frameStarted(const Ogre::FrameEvent& evt)
{
    if (mTraceFrame) std::cout << "VSC::OB::Application frameStarted " << &evt << std::endl;

    //BOOST_ASSERT_MSG (mBulletScene, "Expected mBulletScene");
    
    if (mBulletScene && !mBulletScene->frameStarted(evt.timeSinceLastFrame))
    {
        mBulletScene->shutdown();
        return false;
    }
    
    this->getCameraController()->frameStarted(evt.timeSinceLastFrame);
    
    return true;
}

// -------------------------------------------------------------------------
bool VSC::OB::Application::frameEnded(const Ogre::FrameEvent& evt)
{
    //BOOST_ASSERT_MSG (mBulletScene, "Expected mBulletScene");
    
    if (mTraceFrame) std::cout << "VSC::OB::Application::frameEnded" << std::endl;
    
    if (mBulletScene && !mBulletScene->frameEnded(evt.timeSinceLastFrame))
    {
        mBulletScene->shutdown();
        return false;
    }

    return true; 
}

// -------------------------------------------------------------------------
void VSC::OB::Application::setupResources(void)
{
	VSC::OB::ApplicationBase::setupResources(); 
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
void VSC::OB::Application::loadResources(void)
{
	Ogre::ResourceGroupManager *rsm = Ogre::ResourceGroupManager::getSingletonPtr();
	Ogre::StringVector groups = rsm->getResourceGroups();      
	for (Ogre::StringVector::iterator it = groups.begin(); it != groups.end(); ++it)
	{
		rsm->initialiseResourceGroup((*it));
	}
    
    /*
     *  Initialise, parse scripts etc
     */
    
	//ResourceGroupManager::getSingleton().initialiseAllResourceGroups(); 
}

// -------------------------------------------------------------------------

bool VSC::OB::Application::keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    if (mTraceUI) std::cout << "VSC::OB::Application::keyPressed " << key << std::endl;
    
    return VSC::OB::ApplicationBase::keyPressed(renderWindow, key);
}
