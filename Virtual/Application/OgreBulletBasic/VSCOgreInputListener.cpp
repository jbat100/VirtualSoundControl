

#include "VSCOgreBulletScene.h"
#include "VSCOgreInputListener.h"

#include <boost/assert.hpp>

using Ogre::Vector2;
using Ogre::Real;
using Ogre::RenderWindow;

// -------------------------------------------------------------------------
VSCOgreInputListener::VSCOgreInputListener() 
{

}


void VSCOgreInputListener::mouseMoved(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position, const Ogre::Vector2& movement) {}
void VSCOgreInputListener::mouseEntered(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position) {}
void VSCOgreInputListener::mouseExited(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position) {}
void VSCOgreInputListener::mouseButtonPressed(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position, OIS::MouseButtonID buttonID) {}
void VSCOgreInputListener::mouseButtonReleased(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position, OIS::MouseButtonID buttonID) {}
void VSCOgreInputListener::keyPressed(VSCOgreInputAdapter* adapter, OIS::KeyCode key) {}
void VSCOgreInputListener::keyReleased(VSCOgreInputAdapter* adapter, OIS::KeyCode key) {}

