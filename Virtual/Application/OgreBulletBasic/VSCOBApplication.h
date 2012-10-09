
#ifndef _VSC_OGRE_BULLET_APPLICATION_H_
#define _VSC_OGRE_BULLET_APPLICATION_H_


#include "OgreBulletCollisions.h"
#include "OgreBulletDynamics.h"
#include "VSCOBScene.h"
#include "VSCOBSceneController.h"
#include "VSCOBApplicationBase.h"

#include "VSCIMCollisionMapper.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <vector>

namespace VSC {
    
    namespace OB {
        
        class Application : public ApplicationBase,  public Ogre::FrameListener
        {
            
        public:
            
            typedef boost::shared_ptr<Application>  SPtr;
            typedef boost::weak_ptr<Application>    WPtr;
            
            typedef std::vector<Scene::SPtr>        Scenes;
            
            /*------------------------------------------------------
             *  Constructor / Destructor
             */
            
            Application(Scenes scenes);
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
             *  Scene Controllers
             */
            
            SceneController::SPtr getSceneController(void) {return mSceneController;}
            void setSceneController(SceneController::SPtr controller) {mSceneController = controller;}
            
            CameraController::SPtr getCameraController(void) {return mCameraController;}
            void setCameraController(CameraController::SPtr controller) {mCameraController = controller;}
            
            /*------------------------------------------------------
             *  Collision Listeners
             */
            
            IM::CollisionMapper::SPtr     getCollisionMapper(void) {return mCollisionMapper;}
            void setCollisionMapper(IM::CollisionMapper::SPtr listener) {mCollisionMapper = listener;}
            
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
            
            Scene::SPtr                     mBulletScene;
            Scenes                          mBulletScenes;
            
            SceneController::SPtr           mSceneController;
            
            CameraController::SPtr          mCameraController;
            
            IM::CollisionMapper::SPtr       mCollisionMapper;
            
            static const bool mTraceUI = true;
            static const bool mTraceFrame = false;
            static const bool mTraceScene = true;


        };
        
    }
}

#endif //_VSC_OGRE_BULLET_APPLICATION_H_

