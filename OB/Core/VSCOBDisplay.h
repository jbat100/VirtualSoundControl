
#ifndef _VSC_OB_DISPLAY_H_
#define _VSC_OB_DISPLAY_H_

#include "VSC.h"

#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <map>
#include <string>
#include <vector>


namespace VSC {
    
    namespace OB {
        
        class Scene;
        class Application;

        class Display : public boost::enable_shared_from_this<Display>
        {
            
        private:
            
            typedef boost::shared_ptr<Scene>    Scene_SPtr;
            typedef boost::weak_ptr<Scene>      Scene_WPtr;
            
        public:
            
            friend class Scene;
            friend class Application;
            
            typedef boost::shared_ptr<Display>  SPtr;
            typedef boost::weak_ptr<Display>    WPtr; 
            
            typedef std::map<std::string, std::string>  StatsMap;
            
            virtual ~Display();
            
            /*
             *  Setup and shutdown are not virtual but will call virtual functions
             */
            
            void setupWithScene(Scene_SPtr scene);
            void shutdown();
            
            
            /**--------------------------------------------------------------
             *  Scene
             */
            Scene_SPtr getScene(void) {return mScene.lock();}
            
            bool checkSpaceInFront(Float minDist);
            
            /**--------------------------------------------------------------
             *  Camera
             */
            bool resetCameraAspectRatio(void);
            
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
            
            /**--------------------------------------------------------------
             *  Constructor/Destructor/Initialization
             */
            Display();
            
            virtual void internalSetup() {}          // for subclasses, will be called by setupWithScene()
            virtual void internalShutdown() {}       // for subclasses, will be called by shutdown()
            
            /*
             *  Protected setters
             */
            
            void setRenderWindow(Ogre::RenderWindow* window) {mRenderWindow = window;}
            void setCamera(Ogre::Camera* camera) {mCamera = camera;}
            
            /*
             *  Protected creators
             */
            
            virtual void createCamera(void);
            virtual void createViewport(void);
            virtual void destroyCamera(void);
            virtual void destroyRenderWindow(void);
            
            /*
             *  Utility
             */
            
            Ogre::Vector2 normalizedViewportCoordinates(const Ogre::Vector2& absCoord);
            
        private:
            
            Scene_WPtr                              mScene;
            
            Ogre::RenderWindow*                     mRenderWindow;
            Ogre::Camera*                           mCamera;
            Ogre::ShadowTechnique                   mCurrentShadowTechnique;
            
            StatsMap                                mStatsMap;

            static const bool                       mTrace = true;
            
        };
        
        typedef std::vector<Display::SPtr> Displays;
        
    }
}

#endif //_VSC_OB_DISPLAY_H_

