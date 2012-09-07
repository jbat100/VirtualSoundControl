

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


bool VSCOgreInputListener::mouseMoved(const Ogre::Vector2& position, const Ogre::Vector2& movement) 
{
    if (mTraceUI) std::cout << "VSCOgreInputListener::mouseMoved, next listener is 0x" << (void*)mNextInputListener << std::endl;
    
    if (mNextInputListener)
    {
        return mNextInputListener->mouseMoved(position, movement);
    }
    
    return false;
}

bool VSCOgreInputListener::mouseEntered(const Ogre::Vector2& position) 
{
    if (mNextInputListener)
    {
        return mNextInputListener->mouseEntered(position);
    }
    
    return false;
}

bool VSCOgreInputListener::mouseExited(const Ogre::Vector2& position) 
{
    if (mNextInputListener)
    {
        return mNextInputListener->mouseEntered(position);
    }
    
    return false;
}

bool VSCOgreInputListener::mouseButtonPressed(const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    if (mTraceUI) std::cout << "VSCOgreInputListener::mouseButtonPressed, next listener is 0x" << (void*)mNextInputListener << std::endl;
    
    if (mNextInputListener)
    {
        return mNextInputListener->mouseButtonPressed(position, buttonID);
    }
    
    return false;
}

bool VSCOgreInputListener::mouseButtonReleased(const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    if (mTraceUI) std::cout << "VSCOgreInputListener::mouseButtonReleased, next listener is 0x" << (void*)mNextInputListener << std::endl;
    
    if (mNextInputListener)
    {
        return mNextInputListener->mouseButtonReleased(position, buttonID);
    }
    
    return false;
}

bool VSCOgreInputListener::keyPressed(OIS::KeyCode key)
{
    if (mTraceUI) std::cout << "VSCOgreInputListener::keyPressed " << key << ", next listener is 0x" << (void*)mNextInputListener << std::endl;
    
    if (mNextInputListener)
    {
        return mNextInputListener->keyPressed(key);
    }
    
    return false;
}

bool VSCOgreInputListener::keyReleased(OIS::KeyCode key)
{
    if (mNextInputListener)
    {
        return mNextInputListener->keyReleased(key);
    }
    
    return false;
}


