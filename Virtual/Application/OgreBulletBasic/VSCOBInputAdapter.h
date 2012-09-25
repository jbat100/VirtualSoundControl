
#ifndef _VSC_OGRE_BULLET_INPUT_ADAPTER_H_
#define _VSC_OGRE_BULLET_INPUT_ADAPTER_H_

#include "VSCUI.h"

#include "OIS.h"
#include <Ogre/Ogre.h>

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <boost/enable_shared_from_this.hpp>

#include <set>

namespace VSC {
    
    namespace OB {
        
        class InputListener;
        
        class InputAdapter : public boost::enable_shared_from_this<InputAdapter>
        {
            
        public:
            
            typedef boost::shared_ptr<InputAdapter>     SPtr;
            typedef boost::weak_ptr<InputAdapter>       WPtr;
            
            typedef boost::weak_ptr<InputListener>      InputListenerPtr;
            typedef std::set<InputListenerPtr>          InputListeners;
            
            InputAdapter(void);
            virtual ~InputAdapter(void) {}
            
            /*
             *  Add/Remove input listeners
             */
            
            const InputListeners& getInputListeners(void) {return mInputListeners;}
            void addInputListener(InputListenerPtr listener);
            void removeInputListener(InputListenerPtr listener);
            
            bool areCoordinatesNormalized(void) {return mNormalizedCoordinates;}
            void normalizeCoordinates(bool norm) {mNormalizedCoordinates = norm;}
            
            bool areRapidFireKeyPressesAllowed(void) {return mAllowRapidFireKeyPresses;}
            void allowRapidFireKeyPresses(bool norm) {mAllowRapidFireKeyPresses = norm;}
            
            /*
             *  Listener Keyboard stuff query 
             */
            
            const VSC::KeyCodeSet&  getCurrentKeys() {return mCurrentKeys;} // currently pressed non modifier keys
            bool isKeyPressed(OIS::KeyCode key); 
            OIS::Keyboard::Modifier getCurrentModifier(void) {return mCurrentModifier;} // OIS::Keyboard::Modifier is a bit mask
            
            /*
             *  Listener Mouse stuff query 
             */
            
            bool isMouseButtonPressed(OIS::MouseButtonID) const;
            Ogre::Vector2 getLastMousePosition() const {return mLastMousePosition;}
            Ogre::Vector2 getLastMouseMovement() const {return mLastMouseMovement;}
            Ogre::Vector2 getBufferedMouseMovement() const {return mBufferedMouseMovement;}
            void resetBufferedMouseMovement() {mBufferedMouseMovement = 0;}
            
        protected:
            
            /*
             *  Called by platform specific subclasses to translate from their input detection
             *  mechanisms, will forward calls to the listeners.
             */
            
            /*
             *  Render window changes
             */
            void renderWindowChangedSize(Ogre::RenderWindow* renderWindow);
            
            /*
             *  Mouse input
             */
            void mouseMoved(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, const Ogre::Vector2& movement);
            void mouseEntered(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position);
            void mouseExited(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position);
            void mouseButtonPressed(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID);
            void mouseButtonReleased(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID);
            
            /*
             *  Keyboard input
             */
            void keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key);
            void keyReleased(Ogre::RenderWindow* renderWindow, OIS::KeyCode key);
            void modifierChanged(Ogre::RenderWindow* renderWindow, OIS::Keyboard::Modifier modifier);
            
        private:

            InputListeners                  mInputListeners;
            bool                            mNormalizedCoordinates;
            bool                            mAllowRapidFireKeyPresses;
            
            Ogre::Vector2                   mLastMousePosition;
            Ogre::Vector2                   mLastMouseMovement;
            Ogre::Vector2                   mBufferedMouseMovement;
            
            VSC::KeyCodeSet                 mCurrentKeys;
            VSC::MouseButtonSet             mCurrentMouseButtons;
            
            OIS::Keyboard::Modifier         mCurrentModifier;
            
        };
        
    }
}

#endif//_VSC_OGRE_BULLET_INPUT_ADAPTER_H_

