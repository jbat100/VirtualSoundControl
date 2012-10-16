

#include "VSCOBScene.h"
#include "VSCOBInputListener.h"
#include "VSCOBInputAdapter.h"

#include <boost/assert.hpp>

using Ogre::Vector2;
using Ogre::Real;
using Ogre::RenderWindow;

// -------------------------------------------------------------------------
VSC::OB::InputListener::InputListener() :
mAdapter(InputAdapter::WPtr()),
mNextInputListener(InputListener::WPtr())
{

}

void VSC::OB::InputListener::setInputAdapter(InputAdapter::WPtr adapter) 
{
    mAdapter = adapter;
    
    InputListener::SPtr nextInputListener = mNextInputListener.lock();
    
    /*
     *  Update the responder chain
     */
    if (nextInputListener)
    {
        nextInputListener->setInputAdapter(mAdapter);
    }
}

void VSC::OB::InputListener::setNextInputListener(InputListener::WPtr next)
{
    mNextInputListener = next;
    
    InputListener::SPtr nextInputListener = mNextInputListener.lock();
    
    if (nextInputListener)
    {
        nextInputListener->setInputAdapter(mAdapter);
    }
}


bool VSC::OB::InputListener::mouseMoved(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, const Ogre::Vector2& movement) 
{
    if (mTraceUI) std::cout << "VSC::OB::InputListener::mouseMoved, next listener " << std::endl;
    
    InputListener::SPtr nextInputListener = mNextInputListener.lock();
    
    if (nextInputListener)
    {
        return nextInputListener->mouseMoved(renderWindow, position, movement);
    }
    
    return false;
}

bool VSC::OB::InputListener::mouseEntered(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position) 
{
    InputListener::SPtr nextInputListener = mNextInputListener.lock();
    
    if (nextInputListener)
    {
        return nextInputListener->mouseEntered(renderWindow, position);
    }
    
    return false;
}

bool VSC::OB::InputListener::mouseExited(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position) 
{
    InputListener::SPtr nextInputListener = mNextInputListener.lock();
    
    if (nextInputListener)
    {
        return nextInputListener->mouseEntered(renderWindow, position);
    }
    
    return false;
}

bool VSC::OB::InputListener::mouseButtonPressed(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    if (mTraceUI) std::cout << "VSC::OB::InputListener::mouseButtonPressed, next listener" << std::endl;
    
    InputListener::SPtr nextInputListener = mNextInputListener.lock();
    
    if (nextInputListener)
    {
        return nextInputListener->mouseButtonPressed(renderWindow, position, buttonID);
    }
    
    return false;
}

bool VSC::OB::InputListener::mouseButtonReleased(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    if (mTraceUI) std::cout << "VSC::OB::InputListener::mouseButtonReleased, next listener " << std::endl;
    
    InputListener::SPtr nextInputListener = mNextInputListener.lock();
    
    if (nextInputListener)
    {
        return nextInputListener->mouseButtonReleased(renderWindow, position, buttonID);
    }
    
    return false;
}

bool VSC::OB::InputListener::keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    if (mTraceUI) std::cout << "VSC::OB::InputListener::keyPressed " << key << std::endl;
    
    InputListener::SPtr nextInputListener = mNextInputListener.lock();
    
    if (nextInputListener)
    {
        return nextInputListener->keyPressed(renderWindow, key);
    }
    
    return false;
}

bool VSC::OB::InputListener::keyReleased(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    InputListener::SPtr nextInputListener = mNextInputListener.lock();
    
    if (nextInputListener)
    {
        return nextInputListener->keyReleased(renderWindow, key);
    }
    
    return false;
}

bool VSC::OB::InputListener::modifierChanged(Ogre::RenderWindow* renderWindow, OIS::Keyboard::Modifier modifier)
{
    InputListener::SPtr nextInputListener = mNextInputListener.lock();
    
    if (nextInputListener)
    {
        return nextInputListener->modifierChanged(renderWindow, modifier);
    }
    
    return false;
}

bool VSC::OB::InputListener::renderWindowChangedSize(Ogre::RenderWindow* renderWindow)
{
    InputListener::SPtr nextInputListener = mNextInputListener.lock();
    
    if (nextInputListener)
    {
        return nextInputListener->renderWindowChangedSize(renderWindow);
    }
    
    return false;
}


