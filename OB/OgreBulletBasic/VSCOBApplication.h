
#ifndef _VSC_OB_APPLICATION_H_
#define _VSC_OB_APPLICATION_H_

#include "VSCOBScene.h"
#include "VSCOBDisplay.h"
#include "VSCOBResourceManager.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace OB {
        
        class Application : public Ogre::FrameListener, public boost::enable_shared_from_this<Application>
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
            
            ResourceManager::SPtr getResourceManager(void);
            
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
            
            virtual void setupResources(void);
            virtual void createResourceListener(void); 
            virtual void loadResources(void);

            virtual bool frameStarted(const Ogre::FrameEvent& evt);
            virtual bool frameEnded(const Ogre::FrameEvent& evt);
            
        private:
            
            static void                     InitialiseSingletonApplication();
            static Application::SPtr        mSingletonApplication;
            
            void                            setDisplayRenderWindow(Display::SPtr display, Ogre::RenderWindow* renderWindow);
            
            Bridge::SPtr                    mBridge;
            
            Scenes                          mScenes;
            Displays                        mDisplays;
            ResourceManager::SPtr           mResourceManager;
            
            Ogre::Root*                     mRoot;
            
            bool                            mInitialised;
            
            static const bool               mTraceUI = true;
            static const bool               mTraceFrame = false;
            static const bool               mTraceScene = true;


        };
        
    }
}

#endif //_VSC_OB_APPLICATION_H_

