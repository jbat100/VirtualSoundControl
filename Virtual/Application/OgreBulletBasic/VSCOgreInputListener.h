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
    
    bool isListeningToAdapter(VSCOgreInputAdapter* adapter);
    
    /*
     *  This is the interface which I think should be used
     */
    VSCOgreInputAdapter*    getKeyboardAdapter(void) {return mKeyboardAdapter;}
    VSCOgreInputAdapter*    getMouseAdapter(void) {return mMouseAdapter;}
    void                    setKeyboardAdapter(VSCOgreInputAdapter* keyboardAdapter);
    void                    setMouseAdapter(VSCOgreInputAdapter* mouseAdapter);
    
    /*
     *  Then we can maybe have a set of sensor/glove/joystick listeners
     */
    
    /**--------------------------------------------------------------
     *  These methods are called by the input adapters when some 
     *  input related thing happens. Do not call these methods 
     *  in any other case, unless you want to simulate input.
     *
     *  The decision to not make these methods purely virtual is deliberate,
     *  so that subclasses can choose to implement the methods or not...
     *
     *  The derived classes must call the base class implementation
     *  so that the trackers can be properly updated
     */
    virtual void mouseMoved(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position, const Ogre::Vector2& movement);
    virtual void mouseEntered(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position);
    virtual void mouseExited(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position);
    virtual void mouseButtonPressed(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position, OIS::MouseButtonID buttonID);
    virtual void mouseButtonReleased(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position, OIS::MouseButtonID buttonID);
    virtual void keyPressed(VSCOgreInputAdapter* adapter, OIS::KeyCode key);
    virtual void keyReleased(VSCOgreInputAdapter* adapter, OIS::KeyCode key);
    
protected:
    
    /*
     *  Keeps track of the adapters. I don't think this is really necessary 
     *  as subclasses should be able to distinguish between keyboard adapter,
     *  mouse adapter, joystic adapter, sensor adapter etc...
     */
    
    const std::set<VSCOgreInputAdapter*>& getInputAdapters(void) {return mInputAdapters;}
    
private:
    
    std::set<VSCOgreInputAdapter*>      mInputAdapters;
    
    VSCOgreInputAdapter                 *mKeyboardAdapter;
    VSCOgreInputAdapter                 *mMouseAdapter;
    
};

#endif//_VSC_OGRE_BULLET_INPUT_LISTENER_H_

