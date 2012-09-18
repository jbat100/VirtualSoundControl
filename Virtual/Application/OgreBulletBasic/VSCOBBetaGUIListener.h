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
VSCOgreBetaGUIListener.h
-------------
A basic test framework that minimize code in each test scene listener.
*/
#ifndef _VSC_OGRE_BULLET_GUI_LISTENER_H_
#define _VSC_OGRE_BULLET_GUI_LISTENER_H_

#include <Ogre/Ogre.h>

// We'll need the OgreBulletListener definitions
#include "BetaGUI.h"

class VSC::OB::Scene;

/*
The base Test class, is also able to listen for collisions and thus change the contact properties
*/
class VSC::OB::BetaGUIListener : public BetaGUI::BetaGUIListener
{
public:
	// Constructor/destructor
    VSC::OB::BetaGUIListener(VSC::OB::Scene *listener, Ogre::RenderWindow *win);
    virtual ~VSC::OB::BetaGUIListener();

    // Gui Callbacks
    void onButtonPress(BetaGUI::Button *ref, Ogre::uchar type);

    BetaGUI::GUI *getGui(){return mGui;}

    void addBool(BetaGUI::Window *mainWindow, bool* value, const Ogre::String &label, Ogre::Vector2 &pos);

    void hideMouse();
    void showMouse();
    
    void setMousePosition(Ogre::Vector2 position);
    
    void showHelp(bool show);
    
protected:

   BetaGUI::GUI             *mGui;

   VSC::OB::Scene       *mListener;
   Ogre::RenderWindow       *mWindow;

   Ogre::Overlay		    *mMouseOverlay;
   Ogre::OverlayElement		*mMousePanel;
   Ogre::Real               mMouseCursorHalfWidth;
   Ogre::Real               mMouseCursorHalfHeight;

};

#endif

