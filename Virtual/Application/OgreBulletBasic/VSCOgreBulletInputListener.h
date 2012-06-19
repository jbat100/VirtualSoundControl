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

#include <Ogre/Ogre.h>
#include "OIS.h"
#include "VSCOgreBulletGuiListener.h"
class VSCOgreBulletListener;

class VSCOgreBulletInputListener 
{
    
public:
    
    static const Ogre::Real KEY_DELAY;
    VSCOgreBulletInputListener(VSCOgreBulletListener *ogreBulletListener);
    
    virtual ~VSCOgreBulletInputListener(){};

    // KeyListener Callbacks
    virtual bool keyPressed(const OIS::KeyEvent& e);
    virtual bool keyReleased(const OIS::KeyEvent& e);

    Ogre::Real getRelMouseX() const {return mRelX;}
    Ogre::Real getRelMouseY() const {return mRelY;}

    Ogre::Real getAbsMouseX() const {return mMouseCursorX;}
    Ogre::Real getAbsMouseY() const {return mMouseCursorY;}

    bool getButtonLeftPressed() const {return mButtonLeftPressed;}
    bool getButtonMiddlePressed() const {return mButtonMiddlePressed;}
    bool getButtonRightPressed() const {return mButtonRightPressed;}

protected:

    Ogre::Real mRelX;
    Ogre::Real mRelY;
    Ogre::Real mMouseCursorX;
    Ogre::Real mMouseCursorY;
    
    bool mButtonLeftPressed;
    bool mButtonMiddlePressed;
    bool mButtonRightPressed;

    VSCOgreBulletListener   *mListener;
};

#endif//_VSC_OGRE_BULLET_INPUT_LISTENER_H_

