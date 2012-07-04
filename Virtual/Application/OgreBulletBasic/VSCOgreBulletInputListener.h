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
#include "VSCOgreBulletGuiListener.h"
class VSCOgreBulletListener;

class VSCOgreBulletInputListener : public OIS::MouseListener, public OIS::KeyListener
{
    
public:
    
    typedef std::set<OIS::KeyCode>          KeyCodeSet;
    typedef std::set<OIS::MouseButtonID>    MouseButtonSet;
    
    static const Ogre::Real KEY_DELAY;
    // the listener will be called when something happens, should not be nil
    VSCOgreBulletInputListener(VSCOgreBulletListener *ogreBulletListener, Ogre::RenderWindow *window); 
    virtual ~VSCOgreBulletInputListener(){};

    /*
     *  Listener Keyboard stuff query 
     */
    
    // currently pressed non modifier keys
    const KeyCodeSet&  getCurrentKeys() {return mCurrentKeys;} 
    // convienience method
    bool isKeyPressed(OIS::KeyCode key); 
    bool isKeyModifier(OIS::KeyCode key);

    /*
     *  Listener Mouse stuff query 
     */
    
    bool isMouseButtonPressed(OIS::MouseButtonID) const;
    // probably dont need these ...
    Ogre::Vector2 getLastMousePosition() const {return mLastMousePosition;}
    Ogre::Vector2 getLastMouseMovement() const {return mLastMouseMovement;}
    
    /*
     *  Input called by the actual input system (OIS::MouseListener, public OIS::KeyListener methods)
     *  The can be an OIS input system OR any other such as cocoa (which will have to be implemented)
     */
    
    // MouseMotionListener Callbacks
    bool mouseDragged (const OIS::MouseEvent& e);
    bool mouseMoved   (const OIS::MouseEvent& e);
    
    // MouseListener Callbacks
    bool mouseClicked (const OIS::MouseEvent& e);
    bool mouseEntered (const OIS::MouseEvent& e);
    bool mouseExited  (const OIS::MouseEvent& e);
    
    bool mousePressed (const OIS::MouseEvent& e, OIS::MouseButtonID buttonid);
    bool mouseReleased(const OIS::MouseEvent& e, OIS::MouseButtonID buttonid);
    
    // KeyListener Callbacks
    bool keyClicked(const OIS::KeyEvent& e);
    bool keyPressed(const OIS::KeyEvent& e);
    bool keyReleased(const OIS::KeyEvent& e);
    
protected:

    Ogre::Vector2 mLastMousePosition;
    Ogre::Vector2 mLastMouseMovement;

    KeyCodeSet  mModifierKeys;
    KeyCodeSet  mCurrentKeys;
    
    MouseButtonSet  mCurrentMouseButtons;
    
    VSCOgreBulletListener   *mListener;
    
    Ogre::RenderWindow      *mWindow;
};

#endif//_VSC_OGRE_BULLET_INPUT_LISTENER_H_

