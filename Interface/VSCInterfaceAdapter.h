
#ifndef _VSC_INTERFACE_ADAPTER_H_
#define _VSC_INTERFACE_ADAPTER_H_

#include "VSCUI.h"

#include <boost/shared_ptr.hpp>
#include <boost/weak_ptr.hpp>

#include <boost/enable_shared_from_this.hpp>

#include <vector>

namespace VSC {
    
    template<typename Context, typename Vector2>
    class InterfaceAdapter : public boost::enable_shared_from_this< InterfaceAdapter<Context, Vector2> >
    {
        
    public:
        
        typedef boost::shared_ptr<InterfaceAdapter>  SPtr;
        typedef boost::weak_ptr<InterfaceAdapter>    WPtr;
        
        class Responder {
            
        public:
            
            typedef boost::shared_ptr<Responder>    SPtr;
            typedef boost::weak_ptr<Responder>      WPtr;
            
            Responder() : mAdapter(InterfaceAdapter::WPtr()), mNextResponder(Responder::WPtr()) {}
            virtual ~Responder() {}
            
            void setInterfaceAdapter(InterfaceAdapter::SPtr adapter); //{mAdapter = InterfaceAdapter::WPtr(adapter);}
            InterfaceAdapter::SPtr getInterfaceAdapter(void); //{return mAdapter.lock();}
            
            Responder::SPtr getNextResponder(void) {return mNextResponder.lock();}
            void setNextResponder(Responder::SPtr responder); // {mNextResponder = Responder::WPtr(responder);}
            
            virtual bool contextChanged(const Context* context);
            
            virtual bool modifierChanged(const Context* context, OIS::Keyboard::Modifier modifier);
            
            virtual bool keyPressed(const Context* context, OIS::KeyCode key);
            virtual bool keyReleased(const Context* context, OIS::KeyCode key);
            
            virtual bool mouseMoved(const Context* context, const Vector2& position, const Vector2& movement);
            virtual bool mouseEntered(const Context* context, const Vector2& position);
            virtual bool mouseExited(const Context* context, const Vector2& position);
            virtual bool mouseButtonPressed(const Context* context, const Vector2& position, OIS::MouseButtonID buttonID);
            virtual bool mouseButtonReleased(const Context* context, const Vector2& position, OIS::MouseButtonID buttonID);
            
        private:
            
            InterfaceAdapter::WPtr  mAdapter;
            Responder::WPtr         mNextResponder;
            
            static const bool       mTraceUI;
            
        };
        
        typedef std::vector<typename Responder::SPtr> Responders;
        
        class ResponderChain : public Responder {
            
        public:
            
            typedef boost::shared_ptr<ResponderChain>   SPtr;
            typedef boost::weak_ptr<ResponderChain>     WPtr;
            
            ResponderChain() {}
            virtual ~ResponderChain() {}
            
            const Responders& getResponders(void);
            
            void appendResponder(typename Responder::SPtr responder);
            void prependResponder(typename Responder::SPtr responder);
            void insertResponderAfterResponder(typename Responder::SPtr newResponder, typename Responder::SPtr responder);
            void insertResponderBeforeResponder(typename Responder::SPtr newResponder, typename Responder::SPtr responder);
            
            virtual void removeResponder(typename Responder::SPtr responder);
            virtual void removeAllResponders(void);
            
            bool containsResponder(typename Responder::SPtr responder);
            
        protected:
            
            virtual void chainSceneControllers(void);
            
            virtual void insertResponder(typename Responder::SPtr responder, typename Responders::iterator it);
            
        private:
            
            Responders    mResponders;
            
        };
        
        
        InterfaceAdapter(void);
        virtual ~InterfaceAdapter(void) {}
        
        /*
         *  Add/Remove responders
         */
        
        const Responders& getResponders(void) {return mResponders;}
        void addResponder(typename Responder::SPtr responder);
        void removeResponder(typename Responder::SPtr responder);
        
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
        const Vector2& getLastMousePosition() const {return mLastMousePosition;}
        const Vector2& getLastMouseMovement() const {return mLastMouseMovement;}
        const Vector2& getBufferedMouseMovement() const {return mBufferedMouseMovement;}
        void resetBufferedMouseMovement() {mBufferedMouseMovement = 0;}
        
    protected:
        
        void contextChanged(const Context* context);
        
        /*
         *  Keyboard input
         */
        void keyPressed(const Context* context, OIS::KeyCode key);
        void keyReleased(const Context* context, OIS::KeyCode key);
        void modifierChanged(const Context* context, OIS::Keyboard::Modifier modifier);
        
        /*
         *  Mouse input
         */
        void mouseMoved(const Context* context, const Vector2& position, const Vector2& movement);
        void mouseEntered(const Context* context, const Vector2& position);
        void mouseExited(const Context* context, const Vector2& position);
        void mouseButtonPressed(const Context* context, const Vector2& position, OIS::MouseButtonID buttonID);
        void mouseButtonReleased(const Context* context, const Vector2& position, OIS::MouseButtonID buttonID);
        
    private:

        Responders                      mResponders;
        bool                            mAllowRapidFireKeyPresses;
        bool                            mNormalizedCoordinates;
        
        VSC::KeyCodeSet                 mCurrentKeys;
        VSC::MouseButtonSet             mCurrentMouseButtons;
        
        OIS::Keyboard::Modifier         mCurrentModifier;
        
        Vector2                         mLastMousePosition;
        Vector2                         mLastMouseMovement;
        Vector2                         mBufferedMouseMovement;
        
    };
    
}

#endif //_VSC_INTERFACE_ADAPTER_H_

