
#ifndef _VSC_OGRE_BULLET_SCENE_H_
#define _VSC_OGRE_BULLET_SCENE_H_

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <map>
#include <string>


namespace VSC {
    
    namespace OB {
        
        class Scene;

        class Display
        {
            
        private:
            
            typedef boost::shared_ptr<Scene>    Scene_SPtr;
            typedef boost::weak_ptr<Scene>      Scene_WPtr;
            
        public:
            
            typedef boost::shared_ptr<Display>  SPtr;
            typedef boost::weak_ptr<Display>    WPtr; 
            
            /**--------------------------------------------------------------
             *  Constructor/Destructor/Initialization 
             */
            Display();
            virtual ~Display();
            
            /*
             *  Init and shutdown are not virtual but will call virtual functions
             */
            
            void init(Scene_SPtr scene);
            void shutdown();
            
            /**--------------------------------------------------------------
             *  Scene
             */
            Scene_SPtr getScene(void) {return mScene.lock();}
            
            /**--------------------------------------------------------------
             *  Camera
             */
            bool resetCameraAspectRatio(void);

            /**--------------------------------------------------------------
             *  Ogre Frame Listener Forwarded messages from Application 
             */
            virtual bool frameStarted(Ogre::Real elapsedTime);
            virtual bool frameEnded(Ogre::Real elapsedTime);
            
            /**--------------------------------------------------------------
             *  Ogre Getters
             */

            Ogre::RenderWindow* getRenderWindow(void) {return mWindow;}
            Ogre::Camera* getCamera(void) {return mCamera;}
            
        protected:
            
            /*
             *  Protected setters
             */
            
            void setRenderWindow(Ogre::RenderWindow* window) {mWindow = window;}
            void setCamera(Ogre::Camera* camera) {mCamera = camera;}
            
            /*
             *  Protected creators
             */
            
            virtual void createCamera(void);
            virtual void createRenderWindow(void);
            virtual void createDebugRayLines(void);
            
            /*
             *  Protected destructors
             */
            
            virtual void destroyCamera(void);
            virtual void destroyRenderWindow(void);
            virtual void destroyDebugRayLines(void);
            
        private:
            
            Scene_WPtr                              mScene;
            
            Ogre::RenderWindow                      *mWindow;
            Ogre::Camera                            *mCamera;
            Ogre::ShadowTechnique                   mCurrentShadowTechnique;

            static const bool mTrace = true;
            
        };
        
    }
}

#endif //_VSC_OGRE_BULLET_SCENE_H_

