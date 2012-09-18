
#ifndef _VSC_OGRE_BULLET_APPLICATION_H_
#define _VSC_OGRE_BULLET_APPLICATION_H_

// Include the OgreBullet interface which includes Ogre itself
#include "OgreBulletCollisions.h"
#include "OgreBulletDynamics.h"
#include "VSCOgreBulletScene.h"
#include "VSCOgreApplication.h"

#include <Ogre/Ogre.h>

#include <vector>

namespace VSC {
    
    namespace OB {

        class Application;

        /**
         *  The test application, based on the Ogre example application for consistency
         */
        class Application : public ApplicationBase,  public Ogre::FrameListener
        {
            
        public:
            
            /*------------------------------------------------------
             *  Constructor / Destructor
             */
            
            Application(std::vector<Scene*> scenes);
            ~Application();
            
        #if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            bool setupWithOgreView(void* ogreView);
        #endif   
            
            /*------------------------------------------------------
             *  Scenes stuff
             */
            
            Scene* sceneWithName(Ogre::String name);
            bool switchToScene(Scene *newScene);
            bool switchToSceneWithName(Ogre::String sceneName);
            Scene* getCurrentScene(void) {return mBulletScene;}
            std::vector<Ogre::String> getSceneNames(void);
            
            /*------------------------------------------------------
             *  VSCOgreInputListener override
             */
            
            virtual bool keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key);

        protected:
            
            const std::vector<Scene*>& getScenes() {return mBulletScenes;};
            
            // Override stuff from the base class
            void createScene(void){};	
            void chooseSceneManager(void){};
            void createCamera(void){};
            void createViewports(void){};

            void createFrameListener(void);
            // Add the standard resources, plus our own pack
            void setupResources(void);
            void loadResources(void);

            bool frameStarted(const Ogre::FrameEvent& evt);
            bool frameEnded(const Ogre::FrameEvent& evt);
            
        private:
            
            Scene *mBulletScene;
            std::vector<Scene*> mBulletScenes;
            
            static const bool mTraceUI = true;
            static const bool mTraceFrame = false;
            static const bool mTraceScene = true;


        };
        
    }
}

#endif //_VSC_OGRE_BULLET_APPLICATION_H_

