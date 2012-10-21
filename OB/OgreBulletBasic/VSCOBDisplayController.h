
#ifndef _VSC_OB_DISPLAY_CONTROLLER_H_
#define _VSC_OB_DISPLAY_CONTROLLER_H_

#include "VSCOBInterface.h"
#include "VSCOBKeyBindings.h"
#include "VSCOBKeyboardAction.h"
#include "VSCOBDisplay.h"
#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

namespace VSC {
    
    namespace OB {

        class DisplayController : public InterfaceResponder, public KeyBound
        {
            
        public:
            
            typedef boost::shared_ptr<DisplayController>    SPtr;
            typedef boost::weak_ptr<DisplayController>      WPtr;

            enum CameraMovement {
                CameraMovementNone = 0,
                CameraMovementForward   = 1<<0,
                CameraMovementBackward  = 1<<1,
                CameraMovementLeft      = 1<<2,
                CameraMovementRight     = 1<<3
            };
            
            /**
             *  Constructor/Destructor
             */
            
            DisplayController();
            
            virtual void setupWithDisplay(Display::SPtr display);
            virtual void shutdown(void);

            /**
             *  Ogre Frame Listener Events Forwarded by scene or viewport
             */
            
            virtual bool frameStarted(Ogre::Real elapsedTime);

            /**
             *  Input Listener Overrides, only need to override these
             *  These can be overriden further to provide special camera control...
             */
            
            virtual bool mouseMoved(const Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, const Ogre::Vector2& movement);
            virtual bool keyPressed(const Ogre::RenderWindow* renderWindow, OIS::KeyCode key);
            virtual bool keyReleased(const Ogre::RenderWindow* renderWindow, OIS::KeyCode key);
            virtual bool contextChanged(const Ogre::RenderWindow* renderWindow);
            
            virtual bool renderWindowChangedSize(Ogre::RenderWindow* renderWindow);
            
            /**
             *  Ogre Setters/Getters
             */
            
            Display::SPtr   getDisplay(void) {return mDisplay.lock();}
            
            /**
             *  Camera control
             */
            
            void setCameraPosition(const Ogre::Vector3& position);
            void setCameraPitch(Float pitch);
            void setCameraYaw(Float yaw);
            
            /**
             *  Other Setters/Getters
             */
            
            Float   getMouseSensitivity(void) {return mMouseSensitivity;}
            void    setMouseSensitivity(Float sens) {mMouseSensitivity = sens;}
            Float   getCameraSpeed(void) {return mCameraSpeed;}
            void    setCameraSpeed(Float speed) {mCameraSpeed = speed;}
            
        private:

            Display::WPtr                       mDisplay;
            
            Ogre::Radian                        mCameraRotX;
            Ogre::Radian                        mCameraRotY;
            
            CameraMovement                      mCameraMovement;
            
            //Ogre::Vector3                       mCameraTrans;
            
            Float                               mMouseSensitivity;
            Float                               mCameraSpeed;
            
            const static bool                   mTraceUI = false;
            const static bool                   mTraceFrame = false;
            
        };
        
        typedef std::vector<DisplayController::SPtr> DisplayControllers;
        
        class DisplayControllerChain : public InterfaceResponderChain
        {
            
        public:
            
            void setupWithDisplay(Display::SPtr display);
            void shutdown();
            
            Display::SPtr getDisplay() {return mDisplay.lock();}
            
            virtual void removeResponder(InterfaceResponder::SPtr responder);
            virtual void removeAllResponders(void);
            
        protected:
            
            virtual void insertResponder(InterfaceResponder::SPtr responder, InterfaceResponders::iterator it);
            
        private:
            
            DisplayControllers  mDisplayControllers;
            
            Display::WPtr       mDisplay;
            
        };
        
    }
}

#endif //_VSC_OGRE_CAMERA_CONTROLLER_H_

