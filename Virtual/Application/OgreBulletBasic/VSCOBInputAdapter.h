
#ifndef _VSC_OGRE_BULLET_INPUT_ADAPTER_H_
#define _VSC_OGRE_BULLET_INPUT_ADAPTER_H_

#include "VSCUI.h"
#include "OIS.h"
#include <set>
#include <Ogre/Ogre.h>


namespace VSC {
    
    namespace OB {
        
        class InputListener;
        
        class InputAdapter
        {
            
        public:
            
            InputAdapter(void);
            virtual ~InputAdapter(void) {}
            
            //friend class VSC::OB::InputListener;
            
            /*
             *  Add/Remove input listeners
             */
            
            const std::set<VSC::OB::InputListener*>& getInputListeners(void) {return mInputListeners;}
            void addInputListener(VSC::OB::InputListener* listener);
            void removeInputListener(VSC::OB::InputListener* listener);
            
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

            std::set<VSC::OB::InputListener*>     mInputListeners;
            bool                                mNormalizedCoordinates;
            bool                                mAllowRapidFireKeyPresses;
            
            Ogre::Vector2                       mLastMousePosition;
            Ogre::Vector2                       mLastMouseMovement;
            Ogre::Vector2                       mBufferedMouseMovement;
            
            VSC::KeyCodeSet                       mCurrentKeys;
            VSC::MouseButtonSet                   mCurrentMouseButtons;
            
            OIS::Keyboard::Modifier             mCurrentModifier;
            
        };
        
    }
}

#endif//_VSC_OGRE_BULLET_INPUT_ADAPTER_H_

