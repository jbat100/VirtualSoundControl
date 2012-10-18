
#ifndef _VSC_OB_APPLICATION_H_
#define _VSC_OB_APPLICATION_H_

#include "VSCOBScene.h"

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
            
            typedef std::vector<Scene::SPtr>        Scenes;
            
            /*
             *  Bridge pattern to implement platform dependent functionality
             */
            
            class Bridge {
                
            public:
                
                typedef boost::shared_ptr<Bridge>   SPtr;
                const std::string& getOgreResourcePath(void) {return mOgreResourcePath;}
                virtual Ogre::Root* createOgreRoot();
                
            protected:
                
                void setOgreResourcePath(std::string resourcePath);
                
            private:
                
                std::string mOgreResourcePath; // used as cache, can be overridden in getter
            };
            
            /*------------------------------------------------------
             *  Constructor / Destructor
             */
            
            Application();
            ~Application();
            
            bool init(void); // call init before doing anything
            
            /*------------------------------------------------------
             *  Scenes stuff
             */
            
            void addScene(Scene::SPtr scene);
            void removeScene(Scene::SPtr scene);
            Scene::SPtr sceneWithName(Ogre::String name);
            const Scenes& getScenes(void) {return mScenes;}
            
            
            /*------------------------------------------------------
             *  Ogre Setters/Getters
             */
            
            Ogre::Root* getRoot(void) {return mRoot;}

            const std::string& getOgreResourcePath(void);
            
        protected:
            
            Bridge::SPtr getBridge(void) {return mBridge;}
            
            virtual void interalInit(void) {} // will be called by init() after base init is done
            
            virtual void setupResources(void);
            virtual void createResourceListener(void); 
            virtual void loadResources(void);

            virtual bool frameStarted(const Ogre::FrameEvent& evt);
            virtual bool frameEnded(const Ogre::FrameEvent& evt);
            
        private:
            
            Bridge::SPtr                    mBridge;
            Scenes                          mScenes;
            
            Ogre::Root*                     mRoot;
            
            bool                            mInitialised;
            
            static const bool               mTraceUI = true;
            static const bool               mTraceFrame = false;
            static const bool               mTraceScene = true;


        };
        
    }
}

#endif //_VSC_OB_APPLICATION_H_

