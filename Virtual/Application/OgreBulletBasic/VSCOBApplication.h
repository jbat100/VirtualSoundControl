
#ifndef _VSC_OGRE_BULLET_APPLICATION_H_
#define _VSC_OGRE_BULLET_APPLICATION_H_


#include "OgreBulletCollisions.h"
#include "OgreBulletDynamics.h"
#include "VSCOBScene.h"
#include "VSCOBSceneController.h"
#include "VSCOBApplicationBase.h"

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
            
            typedef std::vector<Scene::SPtr> Scenes;
            
            /*------------------------------------------------------
             *  Constructor / Destructor
             */
            
            Application(std::vector<Scene::SPtr> scenes);
            ~Application();
            
            #if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            bool setupWithOgreView(void* ogreView);
            #endif   
            
            /*------------------------------------------------------
             *  Scenes stuff
             */
            
            Scene::SPtr sceneWithName(Ogre::String name);
            bool switchToScene(Scene::SPtr newScene);
            bool switchToSceneWithName(Ogre::String sceneName);
            Scene::SPtr getCurrentScene(void) {return mBulletScene;}
            std::vector<Ogre::String> getSceneNames(void);
            const Scenes& getScenes(void) {return mBulletScenes;}
            
            /*------------------------------------------------------
             *  SceneController
             */
            
            SceneController::SPtr getSceneController(void);
            void setSceneController(SceneController::SPtr controller);
            
            /*------------------------------------------------------
             *  VSCOgreInputListener override
             */
            
            virtual bool keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key);

        protected:
            
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
            
            Scene::SPtr             mBulletScene;
            Scenes                  mBulletScenes;
            
            SceneController::SPtr   mSceneController;
            
            static const bool mTraceUI = true;
            static const bool mTraceFrame = false;
            static const bool mTraceScene = true;


        };
        
    }
}

#endif //_VSC_OGRE_BULLET_APPLICATION_H_

