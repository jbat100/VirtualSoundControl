
#ifndef _VSC_OB_DISPLAY_H_
#define _VSC_OB_DISPLAY_H_

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <map>
#include <string>


namespace VSC {
    
    namespace OB {
        
        class Scene;

        class Display : public boost::enable_shared_from_this<Display>
        {
            
        private:
            
            typedef boost::shared_ptr<Scene>    Scene_SPtr;
            typedef boost::weak_ptr<Scene>      Scene_WPtr;
            
        public:
            
            typedef boost::shared_ptr<Display>  SPtr;
            typedef boost::weak_ptr<Display>    WPtr; 
            
            typedef std::map<std::string, std::string>  StatsMap;
            
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

            Ogre::RenderWindow* getRenderWindow(void) {return mRenderWindow;}
            Ogre::Camera* getCamera(void) {return mCamera;}
            
            /**--------------------------------------------------------------
             *  Stats Getters
             */
            
            const StatsMap& getUpdatedStatsMap(void);
            
        protected:
            
            /*
             *  Protected setters
             */
            
            void setRenderWindow(Ogre::RenderWindow* window) {mRenderWindow = window;}
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
            
            Ogre::RenderWindow*                     mRenderWindow;
            Ogre::Camera*                           mCamera;
            Ogre::ShadowTechnique                   mCurrentShadowTechnique;
            
            StatsMap                                mStatsMap;

            static const bool                       mTrace = true;
            
        };
        
    }
}

#endif //_VSC_OB_DISPLAY_H_

