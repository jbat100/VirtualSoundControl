
#include "VSCOgreInputAdapter.h"
#include "VSCOgreInputListener.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

VSCOgreInputAdapter::VSCOgreInputAdapter(void) : 
mNormalizedCoordinates(false),
mLastMouseMovement(Ogre::Vector2(0.0, 0.0)),
mLastMousePosition(Ogre::Vector2(0.0, 0.0))
{
    
}

void VSCOgreInputAdapter::addInputListener(VSCOgreInputListener* listener)
{
    BOOST_ASSERT_MSG(listener, "Expected listener");
    mInputListeners.insert(listener);
    listener->setInputAdapter(this);
}

void VSCOgreInputAdapter::removeInputListener(VSCOgreInputListener* listener)
{
    BOOST_ASSERT_MSG(listener, "Expected listener");
    mInputListeners.erase(listener);
    listener->setInputAdapter(this);
}


bool VSCOgreInputAdapter::isKeyPressed(OIS::KeyCode key)
{
    return (mCurrentKeys.find(key) != mCurrentKeys.end());
}


bool VSCOgreInputAdapter::isMouseButtonPressed(OIS::MouseButtonID buttonid) const
{
    return (mCurrentMouseButtons.find(buttonid) != mCurrentMouseButtons.end());
}

void VSCOgreInputAdapter::mouseMoved(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, const Ogre::Vector2& movement)
{
    mLastMousePosition = position;
    mLastMouseMovement = movement;
    mBufferedMouseMovement += movement;
    
    BOOST_FOREACH (VSCOgreInputListener* inputListener, this->getInputListeners()) 
    {
        inputListener->mouseMoved(renderWindow, position, movement);
    }
}


void VSCOgreInputAdapter::mouseEntered(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position)
{
    
}

void VSCOgreInputAdapter::mouseExited(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position)
{
    
}

void VSCOgreInputAdapter::mouseButtonPressed(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    mCurrentMouseButtons.insert(buttonID);
    
    mLastMousePosition = position;
    
    BOOST_FOREACH (VSCOgreInputListener* inputListener, this->getInputListeners()) 
    {
        inputListener->mouseButtonPressed(renderWindow, position, buttonID);
    }
}

void VSCOgreInputAdapter::mouseButtonReleased(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    mCurrentMouseButtons.erase(buttonID);
    
    mLastMousePosition = position;
    
    BOOST_FOREACH (VSCOgreInputListener* inputListener, this->getInputListeners()) 
    {
        inputListener->mouseButtonReleased(renderWindow, position, buttonID);
    }
}

void VSCOgreInputAdapter::keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    bool relayToListeners = true;
    if (mAllowRapidFireKeyPresses == false)
    {
        if (mCurrentKeys.find(key) != mCurrentKeys.end()) 
        {
            relayToListeners = false;
        }
    }
    
    mCurrentKeys.insert(key);
    
    if (relayToListeners) 
    {
        BOOST_FOREACH (VSCOgreInputListener* inputListener, this->getInputListeners()) 
        {
            inputListener->keyPressed(renderWindow, key);
        }
    }

}

void VSCOgreInputAdapter::keyReleased(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
{
    bool relayToListeners = true;
    if (mAllowRapidFireKeyPresses == false)
    {
        if (mCurrentKeys.find(key) == mCurrentKeys.end()) 
        {
            relayToListeners = false;
        }
    }
    
    mCurrentKeys.erase(key);
    
    if (relayToListeners) 
    {
        BOOST_FOREACH (VSCOgreInputListener* inputListener, this->getInputListeners()) 
        {
            inputListener->keyReleased(renderWindow, key);
        }
    }
}

void VSCOgreInputAdapter::modifierChanged(Ogre::RenderWindow* renderWindow, OIS::Keyboard::Modifier modifier)
{
    mCurrentModifier = modifier;
    
    BOOST_FOREACH (VSCOgreInputListener* inputListener, this->getInputListeners()) 
    {
        inputListener->modifierChanged(renderWindow, modifier);
    }
    
}

void VSCOgreInputAdapter::renderWindowChangedSize(Ogre::RenderWindow* renderWindow)
{
    //BOOST_ASSERT_MSG(listener, "Expected listener");
    
    BOOST_FOREACH (VSCOgreInputListener* inputListener, this->getInputListeners())
    {
        inputListener->renderWindowChangedSize(renderWindow);
    }
}


