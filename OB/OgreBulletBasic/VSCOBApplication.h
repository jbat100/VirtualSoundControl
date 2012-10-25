
#ifndef _VSC_OB_APPLICATION_H_
#define _VSC_OB_APPLICATION_H_

#include "VSCOBScene.h"
#include "VSCOBDisplay.h"
#include "VSCOBResourceManager.h"
#include "VSCException.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace OB {
        
        class Application : public boost::enable_shared_from_this<Application>
        {
            
        public:
            
            typedef boost::shared_ptr<Application>  SPtr;
            typedef boost::weak_ptr<Application>    WPtr;
            
            /*
             *  Bridge pattern to implement platform dependent functionality
             */
            
            class Bridge {
                
            public:
                
                typedef boost::shared_ptr<Bridge>   SPtr;
                
                virtual Ogre::Root* createOgreRoot() = 0;
                
                virtual void setupDisplayWithView(Display::SPtr display, void* view) = 0;
                
            protected:
                
                Bridge(Application::SPtr application) : mApplication(Application::WPtr(application)) {}
                
                Application::SPtr getApplication(void) {return mApplication.lock();}
                
                void setDisplayRenderWindow(Display::SPtr display, Ogre::RenderWindow* renderWindow);
                
            private:
                
                Application::WPtr mApplication;

            };
            
            friend class Bridge;
            
            static Application::SPtr singletonApplication(void);
            
            ~Application();
            
            bool init(ResourceManager::SPtr resourceManager); // call init before doing anything
            
            /*------------------------------------------------------
             *  Resource Manager
             */
            
            ResourceManager::SPtr getResourceManager(void) {return mResourceManager;}
            
            /*------------------------------------------------------
             *  Scenes
             */
            
            template<typename SceneSubclass>
            Scene::SPtr createScene(void);
            
            void destroyScene(Scene::SPtr scene);
            Scene::SPtr sceneWithName(Ogre::String name);
            const Scenes& getScenes(void) {return mScenes;}
            
            /*------------------------------------------------------
             *  Displays
             */
            
            template<typename DisplaySubclass>
            Display::SPtr createDisplayWithView(void* view);
            
            void destroyDisplay(Display::SPtr scene);
            const Displays& getDisplays(void) {return mDisplays;}
            
            Display::SPtr getDisplayWithRenderWindow(Ogre::RenderWindow* renderWindow);
            
            
            /*------------------------------------------------------
             *  Ogre Setters/Getters
             */
            
            Ogre::Root* getOgreRoot(void) {return mRoot;}

            const std::string& getOgreResourcePath(void);
            
        protected:
            
            Application();
            
            Bridge::SPtr getBridge(void) {return mBridge;}
            
            virtual void createResourceListener(void) {}
            
        private:
            
            static void                     InitialiseSingletonApplication();
            //static Application::SPtr        mSingletonApplication;
            
            void                            setDisplayRenderWindow(Display::SPtr display, Ogre::RenderWindow* renderWindow);
            
            Bridge::SPtr                    mBridge;
            
            Scenes                          mScenes;
            Displays                        mDisplays;
            ResourceManager::SPtr           mResourceManager;
            
            Ogre::Root*                     mRoot;
            
            Ogre::RenderWindow*             mTestWindow;
            
            bool                            mInitialised;
            
            bool                            mCreatedDisplay;
            bool                            mResourcesLoaded;
            
            static const bool               mTraceUI = true;
            static const bool               mTraceFrame = false;
            static const bool               mTraceScene = true;


        };
        
    }
}

template<typename SceneSubclass>
VSC::OB::Scene::SPtr VSC::OB::Application::createScene()
{
    if (!mCreatedDisplay)
    {
        throw VSCBadStateException("An Ogre RenderWindow must be created before VSC::Scene, create VSC::Display first!");
    }
    
    SceneSubclass* sceneSub = new SceneSubclass();
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
        
        if (!mResourcesLoaded)
        {
            this->getResourceManager()->loadResources();
            mResourcesLoaded = true;
        }
        
        mCreatedDisplay = true;
        
        return sDisplay;
    }
    else
    {
        delete display;
    }
    
    return Display::SPtr();
}

#endif //_VSC_OB_APPLICATION_H_

