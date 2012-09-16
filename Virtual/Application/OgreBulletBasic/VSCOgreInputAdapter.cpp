
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

void VSCOgreInputAdapter::mouseMoved(const Ogre::Vector2& position, const Ogre::Vector2& movement)
{
    mLastMousePosition = position;
    mLastMouseMovement = movement;
    mBufferedMouseMovement += movement;
    
    BOOST_FOREACH (VSCOgreInputListener* inputListener, this->getInputListeners()) 
    {
        inputListener->mouseMoved(position, movement);
    }
}


void VSCOgreInputAdapter::mouseEntered(const Ogre::Vector2& position)
{
    
}

void VSCOgreInputAdapter::mouseExited(const Ogre::Vector2& position)
{
    
}

void VSCOgreInputAdapter::mouseButtonPressed(const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    mCurrentMouseButtons.insert(buttonID);
    
    mLastMousePosition = position;
    
    BOOST_FOREACH (VSCOgreInputListener* inputListener, this->getInputListeners()) 
    {
        inputListener->mouseButtonPressed(position, buttonID);
    }
}

void VSCOgreInputAdapter::mouseButtonReleased(const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    mCurrentMouseButtons.erase(buttonID);
    
    mLastMousePosition = position;
    
    BOOST_FOREACH (VSCOgreInputListener* inputListener, this->getInputListeners()) 
    {
        inputListener->mouseButtonReleased(position, buttonID);
    }
}

void VSCOgreInputAdapter::keyPressed(OIS::KeyCode key)
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
            inputListener->keyPressed(key);
        }
    }

}

void VSCOgreInputAdapter::keyReleased(OIS::KeyCode key)
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
            inputListener->keyReleased(key);
        }
    }
}

void VSCOgreInputAdapter::modifierChanged(OIS::Keyboard::Modifier modifier)
{
    mCurrentModifier = modifier;
    
    BOOST_FOREACH (VSCOgreInputListener* inputListener, this->getInputListeners()) 
    {
        inputListener->modifierChanged(modifier);
    }
    
}


