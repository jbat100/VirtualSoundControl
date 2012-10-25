
#ifndef _VSC_OB_APPLICATION_CPP_
#define _VSC_OB_APPLICATION_CPP_

#include "VSCOBApplication.h"
#include "VSCOBScene.h"

//#ifdef __APPLE__
#include "VSCOBOSXApplicationBridge.h"
//#else
//#error "Unsuported Platform"
//#endif

#include "OIS.h"
#include "OgreResourceGroupManager.h"

#include <Ogre/Ogre.h>
#include <Ogre/OgreConfigFile.h>

#include <boost/assert.hpp>
#include <boost/foreach.hpp>
#include <boost/thread.hpp>

void VSC::OB::Application::Bridge::setDisplayRenderWindow(Display::SPtr display, Ogre::RenderWindow* renderWindow)
{
    if (this->getApplication()) this->getApplication()->setDisplayRenderWindow(display, renderWindow);
}

void VSC::OB::Application::setDisplayRenderWindow(Display::SPtr display, Ogre::RenderWindow* renderWindow)
{
    display->setRenderWindow(renderWindow);
}

boost::once_flag applicationSingletonInitilizedFlag = BOOST_ONCE_INIT;
VSC::OB::Application::SPtr mSingletonApplication;

void VSC::OB::Application::InitialiseSingletonApplication()
{
    BOOST_ASSERT(!mSingletonApplication);
    mSingletonApplication = VSC::OB::Application::SPtr (new VSC::OB::Application);
}


VSC::OB::Application::SPtr VSC::OB::Application::singletonApplication(void)
{
    boost::call_once(&Application::InitialiseSingletonApplication, applicationSingletonInitilizedFlag);
    BOOST_ASSERT(mSingletonApplication);
    return mSingletonApplication;
}

VSC::OB::Application::Application() :
mRoot(0),
mInitialised(false),
mCreatedDisplay(false),
mResourcesLoaded(false),
mTestWindow(0)
{

}
// -------------------------------------------------------------------------
VSC::OB::Application::~Application()
{
    BOOST_FOREACH(Display::SPtr display, mDisplays)
    {
        display->shutdown();
        display->destroyRenderWindow();
    }
    mDisplays.clear();
    
    BOOST_FOREACH(Scene::SPtr scene, mScenes)
    {
        scene->shutdown();
    }
    mScenes.clear();
    
    if (mRoot && mTestWindow)
    {
        mRoot->destroyRenderTarget(mTestWindow);
        mTestWindow = 0;
    }
    
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

bool VSC::OB::Application::init(ResourceManager::SPtr resourceManager)
{
    BOOST_ASSERT(mInitialised == false);
    if (mInitialised) return false;
    
//#ifdef __APPLE__
    mBridge = Bridge::SPtr(new OSXApplicationBridge(shared_from_this()));
//#endif
    
    BOOST_ASSERT(mBridge);
    if (!mBridge) return false;
    BOOST_ASSERT(resourceManager);
    if (!resourceManager) return false;
    
    mResourceManager = resourceManager;
    
    mRoot = mBridge->createOgreRoot();
    BOOST_ASSERT(mRoot);
    
    /*
     *  Enfore the creation of a render window to complete ogre setup process
     */
    //mTestWindow = mRoot->createRenderWindow("TestWindow", 10, 10, false);
    
    mResourceManager->setupResources();
    
    // Need to wait until after a render window is created
    //mResourceManager->loadResources();
    
    mInitialised = true;
    
    return true;
}



void VSC::OB::Application::destroyScene(Scene::SPtr scene)
{
    BOOST_ASSERT(scene);
    if (!scene) return;
    
    Scenes::iterator it = std::find(mScenes.begin(), mScenes.end(), scene);
    BOOST_ASSERT_MSG(it != mScenes.end(), "Scene already not in application");
    if(it != mScenes.end())
    {
        scene->shutdown();
        mScenes.erase(it);
    }
}


void VSC::OB::Application::destroyDisplay(Display::SPtr display)
{
    BOOST_ASSERT(display);
    if (!display) return;
    
    Displays::iterator it = std::find(mDisplays.begin(), mDisplays.end(), display);
    BOOST_ASSERT_MSG(it != mDisplays.end(), "Display not in application");
    if(it != mDisplays.end())
    {
        display->shutdown();
        mDisplays.erase(it);
    }
}

VSC::OB::Display::SPtr VSC::OB::Application::getDisplayWithRenderWindow(Ogre::RenderWindow* renderWindow)
{
    BOOST_FOREACH(Display::SPtr display, mDisplays)
    {
        if (display->getRenderWindow() == renderWindow) return display;
    }
    
    return Display::SPtr();
}

#endif // _VSC_OB_APPLICATION_CPP_

