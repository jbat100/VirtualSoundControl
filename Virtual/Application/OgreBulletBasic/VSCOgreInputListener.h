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
class VSCOgreBulletScene;

class VSCOgreInputListener
{
    
public:
    
    typedef std::set<OIS::KeyCode>          KeyCodeSet;
    typedef std::set<OIS::MouseButtonID>    MouseButtonSet;
    
    static const Ogre::Real KEY_DELAY;
    // the listener will be called when something happens, should not be nil
    VSCOgreInputListener(); 
    virtual ~VSCOgreInputListener(){};

    /*
     *  Listener Keyboard stuff query 
     */
    // currently pressed non modifier keys
    const KeyCodeSet&  getCurrentKeys() {return mCurrentKeys;} 
    // convienience method
    bool isKeyPressed(OIS::KeyCode key); 
    OIS::Keyboard::Modifier currentModifiers(void); // OIS::Keyboard::Modifier is a bit mask
    
    /*
     *  Listener Mouse stuff query 
     */
    bool isMouseButtonPressed(OIS::MouseButtonID) const;
    // probably dont need these ...
    Ogre::Vector2 getLastMousePosition() const {return mLastMousePosition;}
    Ogre::Vector2 getLastMouseMovement() const {return mLastMouseMovement;}
    Ogre::Vector2 getBufferedMouseMovement() const {return mBufferedMouseMovement;}
    void resetBufferedMouseMovement() {mBufferedMouseMovement = 0;}
    
    /**--------------------------------------------------------------
     *  These methods are called by the input adapter when some 
     *  input related thing happens. Do not call these methods 
     *  in any other case, unless you want to simulate input.
     */
    virtual void mouseMoved(const Ogre::Vector2& position, const Ogre::Vector2& movement);
    virtual void mouseEntered(const Ogre::Vector2& position);
    virtual void mouseExited(const Ogre::Vector2& position);
    virtual void mouseButtonPressed(const Ogre::Vector2& position, OIS::MouseButtonID buttonID);
    virtual void mouseButtonReleased(const Ogre::Vector2& position, OIS::MouseButtonID buttonID);
    virtual void keyPressed(OIS::KeyCode key);
    virtual void keyReleased(OIS::KeyCode key);
    
private:

    Ogre::Vector2 mLastMousePosition;
    Ogre::Vector2 mLastMouseMovement;
    Ogre::Vector2 mBufferedMouseMovement;

    KeyCodeSet      mCurrentKeys;
    MouseButtonSet  mCurrentMouseButtons;
    
};

#endif//_VSC_OGRE_BULLET_INPUT_LISTENER_H_

