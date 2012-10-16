
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
        
        class Application : public InputListener, public KeyBound, public Ogre::FrameListener, public boost::enable_shared_from_this<ApplicationBase>
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
                
                const std::string getResourcePath(void) {return mResourcePath;}
                
            private:
                
                std::string mResourcePath;
            };
            
            /*------------------------------------------------------
             *  Constructor / Destructor
             */
            
            Application(const Scenes& scenes);
            ~Application();
            
            bool init(void);
            
            /*------------------------------------------------------
             *  Scenes stuff
             */
            
            void addScene(Scene::SPtr scene);
            void removeScene(Scene::SPtr scene);
            Scene::SPtr sceneWithName(Ogre::String name);
            const Scenes& getScenes(void) {return mScenes;}
            
            void setActiveScene(Scene::SPtr scene);
            
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
            
            IM::CollisionMapper::SPtr getCollisionMapper(void) {return mCollisionMapper;}
            void setCollisionMapper(IM::CollisionMapper::SPtr listener) {mCollisionMapper = listener;}
            
            /*------------------------------------------------------
             *  VSCOgreInputListener override
             */
            
            virtual bool keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key);
            
            /*
             *  Ogre Setters/Getters
             */
            
            Ogre::Root* getRoot(void) {return mRoot;}
            
            /*
             *  Other Setters/Getters
             */
            
            KeyboardManager::SPtr getKeyboardManager(void) const {return mKeyboardManager;}
            void setKeyboardManager(KeyboardManager::SPtr manager) {mKeyboardManager = manager;}


        protected:
            

            virtual bool createOgreRoot(void);
            virtual void setupResources(void);
            
            virtual void createResourceListener(void); 
            virtual void loadResources(void);


            void setupResources(void);
            void loadResources(void);

            bool frameStarted(const Ogre::FrameEvent& evt);
            bool frameEnded(const Ogre::FrameEvent& evt);
            
        private:
            
            Bridge::SPtr                    mBridge;
            
            Scene::SPtr                     mActiveScene;
            Scenes                          mScenes;
            
            SceneController::SPtr           mSceneController;
            CameraController::SPtr          mCameraController;
            IM::CollisionMapper::SPtr       mCollisionMapper;
            
            Ogre::Root*                     mRoot;
            
            KeyboardManager::SPtr           mKeyboardManager;
            
            static const bool               mTraceUI = true;
            static const bool               mTraceFrame = false;
            static const bool               mTraceScene = true;


        };
        
    }
}

#endif //_VSC_OGRE_BULLET_APPLICATION_H_

