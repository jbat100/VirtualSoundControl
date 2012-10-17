
#ifndef _VSC_INTERFACE_ADAPTER_H_
#define _VSC_INTERFACE_ADAPTER_H_

#include "VSCUI.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <boost/enable_shared_from_this.hpp>

#include <vector>

namespace VSC {
    
    template<typename ContextType, typename Vector2DType>
    class InterfaceAdapter : public boost::enable_shared_from_this<InputAdapter>
    {
        
    public:
        
        typedef boost::shared_ptr<InterfaceAdapter>  SPtr;
        typedef boost::weak_ptr<InterfaceAdapter>    WPtr;
        
        class Responder {
            
        public:
            
            typedef boost::shared_ptr<Responder>    SPtr;
            typedef boost::weak_ptr<Responder>      WPtr;
            
            virtual void keyPressed(const ContextType* context, OIS::KeyCode key);
            virtual void keyReleased(const ContextType* context, OIS::KeyCode key);
            virtual void modifierChanged(const ContextType* context, OIS::Keyboard::Modifier modifier);
            
        }
        
        typedef std::vector<Responder::WPtr> Responders;
        
        InterfaceAdapter(void);
        virtual ~InterfaceAdapter(void) {}
        
        /*
         *  Add/Remove responders
         */
        
        const Responders& getResponders(void) {return mResponders;}
        void addResponder(Responder::SPtr responder);
        void removeResponder(Responder::SPtr responder);
        
        /*
         *  Bool Setter/Getters
         */
        
        bool areRapidFireKeyPressesAllowed(void) {return mAllowRapidFireKeyPresses;}
        void allowRapidFireKeyPresses(bool norm) {mAllowRapidFireKeyPresses = norm;}
        
        bool areCoordinatesNormalized(void) {return mNormalizedCoordinates;}
        void normalizeCoordinates(bool norm) {mNormalizedCoordinates = norm;}
        
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
        const Vector2DType& getLastMousePosition() const {return mLastMousePosition;}
        const Vector2DType& getLastMouseMovement() const {return mLastMouseMovement;}
        const Vector2DType& getBufferedMouseMovement() const {return mBufferedMouseMovement;}
        void resetBufferedMouseMovement() {mBufferedMouseMovement = 0;}
        
    protected:
        
        /*
         *  Keyboard input
         */
        void keyPressed(const ContextType* context, OIS::KeyCode key);
        void keyReleased(const ContextType* context, OIS::KeyCode key);
        void modifierChanged(const ContextType* context, OIS::Keyboard::Modifier modifier);
        
        /*
         *  Mouse input
         */
        void mouseMoved(const ContextType* context, const Vector2DType& position, const Vector2DType& movement);
        void mouseEntered(const ContextType* context, const Vector2DType& position);
        void mouseExited(const ContextType* context, const Vector2DType& position);
        void mouseButtonPressed(const ContextType* context, const Vector2DType& position, OIS::MouseButtonID buttonID);
        void mouseButtonReleased(const ContextType* context, const Vector2DType& position, OIS::MouseButtonID buttonID);
        
    private:

        Responders                      mResponders;
        bool                            mAllowRapidFireKeyPresses;
        
        VSC::KeyCodeSet                 mCurrentKeys;
        VSC::MouseButtonSet             mCurrentMouseButtons;
        
        OIS::Keyboard::Modifier         mCurrentModifier;
        
        Vector2DType                    mLastMousePosition;
        Vector2DType                    mLastMouseMovement;
        Vector2DType                    mBufferedMouseMovement;
        
    };
    
}

#endif //_VSC_INTERFACE_ADAPTER_H_

