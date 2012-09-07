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
class VSCOgreInputAdapter;

class VSCOgreInputListener
{
    
public:
    
    // the listener will be called when something happens, should not be nil
    VSCOgreInputListener(); 
    virtual ~VSCOgreInputListener(){}
    
    friend class VSCOgreInputAdapter;
    
    /*----------------------------------------------------------------
     *  This is the interface which I think should be used
     */
    VSCOgreInputAdapter*    getInputAdapter(void) {return mAdapter;}
    void                    setInputAdapter(VSCOgreInputAdapter* adapter);
    
    /*
     *  Responder chain, if the current listener does not respond to an interface event (returns false)
     *  then the call will be forwarded down the chain.
     */
    VSCOgreInputListener*   getNextInputListener(void) {return mNextInputListener;}
    void                    setNextInputListener(VSCOgreInputListener* next);
    
    /**--------------------------------------------------------------
     *  These methods are called by the input adapters when some 
     *  input related thing happens, then chained down the responders. 
     *  Do not call these methods 
     *  in any other case, unless you want to simulate input.
     *
     *  The decision to not make these methods purely virtual is deliberate,
     *  so that subclasses can choose to implement the methods or not...
     */

    virtual bool mouseMoved(const Ogre::Vector2& position, const Ogre::Vector2& movement);
    virtual bool mouseEntered(const Ogre::Vector2& position);
    virtual bool mouseExited(const Ogre::Vector2& position);
    virtual bool mouseButtonPressed(const Ogre::Vector2& position, OIS::MouseButtonID buttonID);
    virtual bool mouseButtonReleased(const Ogre::Vector2& position, OIS::MouseButtonID buttonID);
    virtual bool keyPressed(OIS::KeyCode key);
    virtual bool keyReleased(OIS::KeyCode key);
    
private:
    
    VSCOgreInputAdapter*                mAdapter;
    VSCOgreInputListener*               mNextInputListener;
    
    static const bool                   mTraceUI = true;
    
};

#endif//_VSC_OGRE_BULLET_INPUT_LISTENER_H_

