

#include "VSCOBScene.h"
#include "VSCOBInputListener.h"
#include "VSCOBInputAdapter.h"

#include <boost/assert.hpp>

using Ogre::Vector2;
using Ogre::Real;
using Ogre::RenderWindow;

// -------------------------------------------------------------------------
VSC::OB::InputListener::InputListener() :
mAdapter(0),
mNextInputListener(0)
{

}

void VSC::OB::InputListener::setInputAdapter(VSC::OB::InputAdapter* adapter) 
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

void VSC::OB::InputListener::setNextInputListener(VSC::OB::InputListener* next)
{
    mNextInputListener = next;
    
    if (mNextInputListener)
    {
        mNextInputListener->setInputAdapter(mAdapter);
    }
}


bool VSC::OB::InputListener::mouseMoved(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, const Ogre::Vector2& movement) 
{
    if (mTraceUI) std::cout << "VSC::OB::InputListener::mouseMoved, next listener is 0x" << (void*)mNextInputListener << std::endl;
    
    if (mNextInputListener)
    {
        return mNextInputListener->mouseMoved(renderWindow, position, movement);
    }
    
    return false;
}

bool VSC::OB::InputListener::mouseEntered(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position) 
{
    if (mNextInputListener)
    {
        return mNextInputListener->mouseEntered(renderWindow, position);
    }
    
    return false;
}

bool VSC::OB::InputListener::mouseExited(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position) 
{
    if (mNextInputListener)
    {
        return mNextInputListener->mouseEntered(renderWindow, position);
    }
    
    return false;
}

bool VSC::OB::InputListener::mouseButtonPressed(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    if (mTraceUI) std::cout << "VSC::OB::InputListener::mouseButtonPressed, next listener is 0x" << (void*)mNextInputListener << std::endl;
    
    if (mNextInputListener)
    {
        return mNextInputListener->mouseButtonPressed(renderWindow, position, buttonID);
    }
    
    return false;
}

bool VSC::OB::InputListener::mouseButtonReleased(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    if (mTraceUI) std::cout << "VSC::OB::InputListener::mouseButtonReleased, next listener is 0x" << (void*)mNextInputListener << std::endl;
    
    if (mNextInputListener)
    {
        return mNextInputListener->mouseButtonReleased(renderWindow, position, buttonID);
    }
    
    return false;
}

bool VSC::OB::InputListener::keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    if (mTraceUI) std::cout << "VSC::OB::InputListener::keyPressed " << key << ", next listener is 0x" << (void*)mNextInputListener << std::endl;
    
    if (mNextInputListener)
    {
        return mNextInputListener->keyPressed(renderWindow, key);
    }
    
    return false;
}

bool VSC::OB::InputListener::keyReleased(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    if (mNextInputListener)
    {
        return mNextInputListener->keyReleased(renderWindow, key);
    }
    
    return false;
}

bool VSC::OB::InputListener::modifierChanged(Ogre::RenderWindow* renderWindow, OIS::Keyboard::Modifier modifier)
{
    if (mNextInputListener)
    {
        return mNextInputListener->modifierChanged(renderWindow, modifier);
    }
    
    return false;
}

bool VSC::OB::InputListener::renderWindowChangedSize(Ogre::RenderWindow* renderWindow)
{
    if (mNextInputListener)
    {
        return mNextInputListener->renderWindowChangedSize(renderWindow);
    }
    
    return false;
}


