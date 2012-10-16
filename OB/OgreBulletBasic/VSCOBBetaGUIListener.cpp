/***************************************************************************
This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10
Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)
This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/

#include "VSCOBScene.h"
#include "VSCOBBetaGUIListener.h"


using namespace Ogre;

// -------------------------------------------------------------------------
VSC::OB::BetaGUIListener::BetaGUIListener(VSC::OB::Scene *listener, Ogre::RenderWindow *win) :
    mListener(listener),
    mWindow(win),
    mMouseOverlay(0),
    mMousePanel(0)
{	

    /******************* CREATE Cursor Overlay ***************************/
    mMouseOverlay = (Overlay*)OverlayManager::getSingleton().getByName("GuiOverlay");
    if (mMouseOverlay)
    {
        mMousePanel = OverlayManager::getSingleton().getOverlayElement ("GUIMouse");
    }
    else
    {
        mMouseOverlay = OverlayManager::getSingletonPtr()->create("GuiOverlay");
        mMouseOverlay->setZOrder(600);
        mMousePanel = (Ogre::OverlayElement *)
            OverlayManager::getSingletonPtr()->createOverlayElement("Panel", "GUIMouse");
        
        try {
            mMousePanel->setMaterialName("OgreBulletDemos/TargetSights");
        }
        catch (Ogre::ItemIdentityException& e) {
            std::cout << "Exception: " << e.getFullDescription() << std::endl;
        }

        TexturePtr mouseTex = TextureManager::getSingleton().load("target.png", "Bullet");
        mMousePanel->setWidth (mouseTex->getWidth() / (float)win->getWidth());
        mMousePanel->setHeight (mouseTex->getHeight() / (float)win->getHeight());

        Ogre::OverlayContainer		*mouseContainer = (Ogre::OverlayContainer*) 
            OverlayManager::getSingletonPtr()->createOverlayElement("Panel", "GUIContainer");
        mMouseOverlay->add2D(mouseContainer);
        mouseContainer->addChild(mMousePanel);
    }

    mMouseOverlay->show(); 
    TexturePtr mouseTex = TextureManager::getSingleton().load("target.png", "Bullet");
    mMouseCursorHalfWidth = (Real(mouseTex->getWidth()) / mWindow->getWidth()) * 0.5;
    mMouseCursorHalfHeight = (Real(mouseTex->getHeight())  / mWindow->getHeight ()) * 0.5;

    /******************* CREATE GUI ***************************/
    mGui = new BetaGUI::GUI("OgreBulletGui", "BlueHighway", 14, win);
    OverlayContainer* mouseCursor = mGui->createMousePointer(Vector2(32, 32), "bgui.pointer");
    mouseCursor->hide();

    std::cout << "Window dimensions: " << mWindow->getWidth() << " " << mWindow->getHeight() << std::endl;
    
    mGui->injectMouse(mWindow->getWidth() * 0.5, mWindow->getHeight() * 0.5, false);

}
// -------------------------------------------------------------------------
VSC::OB::BetaGUIListener::~BetaGUIListener()
{
    // other buttons  and window managed by BETAGUI will be deleted by BETAGUI himself.
    delete mGui;
}
// -------------------------------------------------------------------------
void VSC::OB::BetaGUIListener::addBool(BetaGUI::Window *window, bool* value, const String &label, Vector2 &pos)
{
    window->createBoolButton(
        Vector4(pos.x, pos.y, 7*label.size(), 24), 
        "bgui.button", 
        label, 
        BetaGUI::Callback(this), 
        value);
}
// -------------------------------------------------------------------------
void VSC::OB::BetaGUIListener::onButtonPress(BetaGUI::Button *ref, Ogre::uchar type)
{
    //if(type == 1) // button down
    //{
        //      
    //}
}
// -------------------------------------------------------------------------
void VSC::OB::BetaGUIListener::hideMouse()
{
   mMousePanel->hide();
}
// -------------------------------------------------------------------------
void VSC::OB::BetaGUIListener::showMouse()
{
   mMousePanel->show();
}
// -------------------------------------------------------------------------
void VSC::OB::BetaGUIListener::setMousePosition(Ogre::Vector2 position)
{
    mMousePanel->setPosition (position.x - mMouseCursorHalfWidth, position.y - mMouseCursorHalfHeight);
}
