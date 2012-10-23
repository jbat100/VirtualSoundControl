
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

void VSC::OB::Application::InitialiseSingletonApplication()
{
    BOOST_ASSERT(!singletonApplication);
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
Ogre::FrameListener(),
mInitialised(false)
{

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

bool VSC::OB::Application::init(ResourceManager::SPtr resourceManager)
{
    BOOST_ASSERT(mInitialised == false);
    if (mInitialised) return false;
    
#ifdef __APPLE__
    mBridge = Bridge::SPtr(new OSXApplicationBridge(shared_from_this()));
#endif
    
    BOOST_ASSERT(mBridge);
    if (!mBridge) return false;
    
    mRoot = mBridge->createOgreRoot();
    BOOST_ASSERT(mRoot);
    
    BOOST_ASSERT(resourceManager);
    if (!resourceManager) return false;
    mResourceManager = resourceManager;
    
    mResourceManager->setupResources();
    mResourceManager->loadResources();
    
    mRoot->addFrameListener(this);
    
    mInitialised = true;
    
    return true;
}

template<typename SceneSubclass>
VSC::OB::Scene::SPtr VSC::OB::Application::createScene()
{
    SceneSubclass* sceneSub = new SceneSubclass(shared_from_this());
    Scene* scene = dynamic_cast<Scene*>(sceneSub);
    BOOST_ASSERT(scene);
    
    if (scene)
    {
        Scene::SPtr sscene = Scene::SPtr(scene);
        sscene->init();
        mScenes.push_back(sscene);
        return sscene;
    }
    else
    {
        delete sceneSub;
    }
    
    return Scene::SPtr();
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

template<typename DisplaySubclass>
VSC::OB::Display::SPtr VSC::OB::Application::createDisplayWithView(void* view)
{
    BOOST_ASSERT(mBridge);
    if (!mBridge) return Display::SPtr();
    
    DisplaySubclass* displaySub = new DisplaySubclass();
    Display* display = dynamic_cast<Display*>(displaySub);
    BOOST_ASSERT(display);
    
    if (display)
    {
        Display::SPtr sDisplay = Display::SPtr(display);
        mBridge->setupDisplayWithView(sDisplay, view);
        mDisplays.push_back(sDisplay);
        return sDisplay;
    }
    else
    {
        delete display;
    }
    
    return Display::SPtr();
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

bool VSC::OB::Application::frameStarted(const Ogre::FrameEvent& evt)
{
    if (mTraceFrame) std::cout << "VSC::OB::Application frameStarted " << &evt << std::endl;

    BOOST_FOREACH(Scene::SPtr scene, this->getScenes())
    {
        scene->frameStarted(evt.timeSinceLastFrame);
    }
    
    return true;
}


bool VSC::OB::Application::frameEnded(const Ogre::FrameEvent& evt)
{
    
    if (mTraceFrame) std::cout << "VSC::OB::Application::frameEnded" << std::endl;
    
    BOOST_FOREACH(Scene::SPtr scene, this->getScenes())
    {
        scene->frameEnded(evt.timeSinceLastFrame);
    }

    return true; 
}



