

#ifndef _VSC_OGRE_APPLICATION_H_
#define _VSC_OGRE_APPLICATION_H_

#include "VSCOBInputListener.h"
#include "VSCOBKeyboardManager.h"
#include "VSCOBKeyBindings.h"

#include <Ogre/Ogre.h>
#include <Ogre/OgreConfigFile.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <boost/enable_shared_from_this.hpp>

namespace VSC {
    
    namespace OB {

        class InputAdapter;

        /**
         *  Base class which manages the standard startup of an Ogre application.
         *  Designed to be subclassed for specific examples if required.
         */
        class ApplicationBase : public InputListener, public KeyBound, public boost::enable_shared_from_this<ApplicationBase>
        {
            
        public:
            
            typedef boost::shared_ptr<ApplicationBase>  SPtr;
            typedef boost::weak_ptr<ApplicationBase>    WPtr;
            
#if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            friend class OSXApplicationSetup;
#endif
            
            /// Standard constructor
            ApplicationBase();
            /// Standard destructor
            virtual ~ApplicationBase();
            
            #if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
            friend class OSXApplicationSetup;
            virtual bool setupWithOgreView(void* ogreView);
            #else   
            /// Start the example
            virtual void go(void);
            // These internal methods package up the stages in the startup process
            /** Sets up the application - returns false if the user chooses to abandon configuration. */
            virtual bool setup(void);
            #endif
            
            /*
             *  Ogre Setters/Getters
             */
            
            Ogre::Root*             getRoot(void) {return mRoot;}
            Ogre::Camera*           getCamera(void) {return mCamera;}
            Ogre::SceneManager*     getSceneManager(void) {return mSceneMgr;}
            Ogre::RenderWindow*     getRenderWindow(void) {return mWindow;}
            
            /*
             *  Other Setters/Getters
             */
            
            KeyboardManager::SPtr    getKeyboardManager(void) const {return mKeyboardManager;}
            void                     setKeyboardManager(KeyboardManager::SPtr manager) {mKeyboardManager = manager;}
            
            /*
             *  InputListener override
             */
            virtual bool keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key);

        protected:
            
            /*
             *  NOTE: This is probably where the fun is in terms of making the application run on Cocoa
             */
            
            /** Configures the application - returns false if the user chooses to abandon configuration. */
            virtual bool createOgreRoot(void);
            virtual void createSceneManager(void);
            virtual void createCamera(void);

            
            virtual void createViewports(void);
            
            virtual void setupResources(void); // Method which will define the source of resources (other than current folder)
            virtual void createResourceListener(void); // Optional override method where you can create resource listeners (e.g. for loading screens)
            /// Optional override method where you can perform resource group loading
            /// Must at least do ResourceGroupManager::getSingleton().initialiseAllResourceGroups();
            virtual void loadResources(void);
            
        private:
            
            Ogre::Root                      *mRoot;
            Ogre::Camera                    *mCamera;
            Ogre::SceneManager              *mSceneMgr;
            Ogre::RenderWindow              *mWindow;
            Ogre::String                    mResourcePath;
            
            KeyboardManager::SPtr mKeyboardManager;
            
            static const bool mTraceUI = true;

        };
        
    }
}


#endif//_VSC_EXAMPLE_APPLICATION_H_
