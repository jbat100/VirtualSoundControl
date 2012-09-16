

#include "VSCOgreBulletScene.h"
#include "VSCOgreInputListener.h"
#include "VSCOgreInputAdapter.h"

#include <boost/assert.hpp>

using Ogre::Vector2;
using Ogre::Real;
using Ogre::RenderWindow;

// -------------------------------------------------------------------------
VSCOgreInputListener::VSCOgreInputListener() :
mAdapter(0),
mNextInputListener(0)
{

}

void VSCOgreInputListener::setInputAdapter(VSCOgreInputAdapter* adapter) 
{
    mAdapter = adapter;
    
    /*
     *  Update the responder chain
     */
    if (mNextInputListener)
    {
        mNextInputListener->setInputAdapter(mAdapter);
    }
}

void VSCOgreInputListener::setNextInputListener(VSCOgreInputListener* next)
{
    mNextInputListener = next;
    
    if (mNextInputListener)
    {
        mNextInputListener->setInputAdapter(mAdapter);
    }
}


bool VSCOgreInputListener::mouseMoved(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, const Ogre::Vector2& movement) 
{
    if (mTraceUI) std::cout << "VSCOgreInputListener::mouseMoved, next listener is 0x" << (void*)mNextInputListener << std::endl;
    
    if (mNextInputListener)
    {
        return mNextInputListener->mouseMoved(renderWindow, position, movement);
    }
    
    return false;
}

bool VSCOgreInputListener::mouseEntered(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position) 
{
    if (mNextInputListener)
    {
        return mNextInputListener->mouseEntered(renderWindow, position);
    }
    
    return false;
}

bool VSCOgreInputListener::mouseExited(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position) 
{
    if (mNextInputListener)
    {
        return mNextInputListener->mouseEntered(renderWindow, position);
    }
    
    return false;
}

bool VSCOgreInputListener::mouseButtonPressed(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    if (mTraceUI) std::cout << "VSCOgreInputListener::mouseButtonPressed, next listener is 0x" << (void*)mNextInputListener << std::endl;
    
    if (mNextInputListener)
    {
        return mNextInputListener->mouseButtonPressed(renderWindow, position, buttonID);
    }
    
    return false;
}

bool VSCOgreInputListener::mouseButtonReleased(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    if (mTraceUI) std::cout << "VSCOgreInputListener::mouseButtonReleased, next listener is 0x" << (void*)mNextInputListener << std::endl;
    
    if (mNextInputListener)
    {
        return mNextInputListener->mouseButtonReleased(renderWindow, position, buttonID);
    }
    
    return false;
}

bool VSCOgreInputListener::keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    if (mTraceUI) std::cout << "VSCOgreInputListener::keyPressed " << key << ", next listener is 0x" << (void*)mNextInputListener << std::endl;
    
    if (mNextInputListener)
    {
        return mNextInputListener->keyPressed(renderWindow, key);
    }
    
    return false;
}

bool VSCOgreInputListener::keyReleased(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    if (mNextInputListener)
    {
        return mNextInputListener->keyReleased(renderWindow, key);
    }
    
    return false;
}

bool VSCOgreInputListener::modifierChanged(Ogre::RenderWindow* renderWindow, OIS::Keyboard::Modifier modifier)
{
    if (mNextInputListener)
    {
        return mNextInputListener->modifierChanged(renderWindow, modifier);
    }
    
    return false;
}

bool VSCOgreInputListener::renderWindowChangedSize(Ogre::RenderWindow* renderWindow)
{
    if (mNextInputListener)
    {
        return mNextInputListener->renderWindowChangedSize(renderWindow);
    }
    
    return false;
}


