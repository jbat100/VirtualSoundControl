

#include "VSCOgreBulletScene.h"
#include "VSCOgreInputListener.h"
#include "VSCOgreInputAdapter.h"

#include <boost/assert.hpp>

using Ogre::Vector2;
using Ogre::Real;
using Ogre::RenderWindow;

// -------------------------------------------------------------------------
VSCOgreInputListener::VSCOgreInputListener() :
mKeyboardAdapter(0),
mMouseAdapter(0)
{

}

bool VSCOgreInputListener::isListeningToAdapter(VSCOgreInputAdapter* adapter)
{
    if (!adapter) 
    {
        return false;
    }
    
    if (adapter == mKeyboardAdapter) 
    {
        return true;
    }
    
    if (adapter == mMouseAdapter)
    {
        return true;
    }
    
    if (mInputAdapters.find(adapter) != mInputAdapters.end()) 
    {
        return true;
    }
    
    return false;
    
}

void VSCOgreInputListener::setKeyboardAdapter(VSCOgreInputAdapter* keyboardAdapter) 
{
    VSCOgreInputAdapter* oldAdapter = mKeyboardAdapter;
    mKeyboardAdapter = keyboardAdapter;
    
    if (oldAdapter && this->isListeningToAdapter(oldAdapter)) 
    {
        oldAdapter->removeInputListener(this);
    }
    
    if (mKeyboardAdapter)
    {
        mKeyboardAdapter->addInputListener(this);
    }
}

void VSCOgreInputListener::setMouseAdapter(VSCOgreInputAdapter* mouseAdapter) 
{
    VSCOgreInputAdapter* oldAdapter = mMouseAdapter;
    mMouseAdapter = mouseAdapter;
    
    if (oldAdapter && this->isListeningToAdapter(oldAdapter)) 
    {
        mMouseAdapter->removeInputListener(this);
    }
    
    if (mMouseAdapter)
    {
        mMouseAdapter->addInputListener(this);
    }
}


void VSCOgreInputListener::mouseMoved(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position, const Ogre::Vector2& movement) {}
void VSCOgreInputListener::mouseEntered(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position) {}
void VSCOgreInputListener::mouseExited(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position) {}
void VSCOgreInputListener::mouseButtonPressed(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position, OIS::MouseButtonID buttonID) {}
void VSCOgreInputListener::mouseButtonReleased(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position, OIS::MouseButtonID buttonID) {}
void VSCOgreInputListener::keyPressed(VSCOgreInputAdapter* adapter, OIS::KeyCode key) {}
void VSCOgreInputListener::keyReleased(VSCOgreInputAdapter* adapter, OIS::KeyCode key) {}

