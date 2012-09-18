/***************************************************************************
This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10
Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)
This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/
/***************************************************************************
 File modified for VSC project
 -----------------------------------------------------------------------------*/

/*
InputListener.h
-------------
A basic test framework that minimize code in each test scene listener.
*/
#ifndef _VSC_OGRE_BULLET_INPUT_LISTENER_H_
#define _VSC_OGRE_BULLET_INPUT_LISTENER_H_

#include <set>
#include <Ogre/Ogre.h>
#include "OIS.h"

namespace VSC {
    
    namespace OB {

        class InputAdapter;

        class InputListener
        {
            
        public:
            
            // the listener will be called when something happens, should not be nil
            InputListener(); 
            virtual ~InputListener(){}
            
            friend class VSC::OB::InputAdapter;
            
            /*----------------------------------------------------------------
             *  This is the interface which I think should be used
             */
            VSC::OB::InputAdapter*    getInputAdapter(void) {return mAdapter;}
            void                    setInputAdapter(VSC::OB::InputAdapter* adapter);
            
            /*
             *  Responder chain, if the current listener does not respond to an interface event (returns false)
             *  then the call will be forwarded down the chain.
             */
            VSC::OB::InputListener*   getNextInputListener(void) {return mNextInputListener;}
            void                    setNextInputListener(VSC::OB::InputListener* next);
            
            /**--------------------------------------------------------------
             *  These methods are called by the input adapters when some 
             *  input related thing happens, then chained down the responders. 
             *  Do not call these methods 
             *  in any other case, unless you want to simulate input.
             *
             *  The decision to not make these methods purely virtual is deliberate,
             *  so that subclasses can choose to implement the methods or not...
             */

            virtual bool mouseMoved(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, const Ogre::Vector2& movement);
            virtual bool mouseEntered(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position);
            virtual bool mouseExited(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position);
            virtual bool mouseButtonPressed(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID);
            virtual bool mouseButtonReleased(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID);
            
            virtual bool keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key);
            virtual bool keyReleased(Ogre::RenderWindow* renderWindow, OIS::KeyCode key);
            
            virtual bool modifierChanged(Ogre::RenderWindow* renderWindow, OIS::Keyboard::Modifier modifier);
            
            virtual bool renderWindowChangedSize(Ogre::RenderWindow* renderWindow);
            
        private:
            
            VSC::OB::InputAdapter*                mAdapter;
            VSC::OB::InputListener*               mNextInputListener;
            
            static const bool                   mTraceUI = true;
            
        };
    }
}

#endif//_VSC_OGRE_BULLET_INPUT_LISTENER_H_

