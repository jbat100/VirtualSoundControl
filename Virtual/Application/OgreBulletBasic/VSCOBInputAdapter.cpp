
#include "VSCOBInputAdapter.h"
#include "VSCOBInputListener.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>

VSC::OB::InputAdapter::InputAdapter(void) : 
mNormalizedCoordinates(false),
mLastMouseMovement(Ogre::Vector2(0.0, 0.0)),
mLastMousePosition(Ogre::Vector2(0.0, 0.0))
{
    
}

void VSC::OB::InputAdapter::addInputListener(InputListener::WPtr listener)
{
    BOOST_ASSERT_MSG(listener.lock(), "Expected listener");
    mInputListeners.insert(listener);
    listener.lock()->setInputAdapter(InputAdapter::WPtr(shared_from_this()));
}

void VSC::OB::InputAdapter::removeInputListener(InputListener::WPtr listener)
{
    BOOST_ASSERT_MSG(listener.lock(), "Expected listener");
    mInputListeners.erase(listener);
    listener.lock()->setInputAdapter(InputAdapter::WPtr());
}


bool VSC::OB::InputAdapter::isKeyPressed(OIS::KeyCode key)
{
    return (mCurrentKeys.find(key) != mCurrentKeys.end());
}


bool VSC::OB::InputAdapter::isMouseButtonPressed(OIS::MouseButtonID buttonid) const
{
    return (mCurrentMouseButtons.find(buttonid) != mCurrentMouseButtons.end());
}

void VSC::OB::InputAdapter::mouseMoved(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, const Ogre::Vector2& movement)
{
    mLastMousePosition = position;
    mLastMouseMovement = movement;
    mBufferedMouseMovement += movement;
    
    BOOST_FOREACH (InputListener::WPtr inputListener, this->getInputListeners())
    {
        InputListener::SPtr sinputListener = inputListener.lock();
        if (sinputListener) {
            sinputListener->mouseMoved(renderWindow, position, movement);
        }
    }
}


void VSC::OB::InputAdapter::mouseEntered(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position)
{
    
}

void VSC::OB::InputAdapter::mouseExited(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position)
{
    
}

void VSC::OB::InputAdapter::mouseButtonPressed(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    mCurrentMouseButtons.insert(buttonID);
    
    mLastMousePosition = position;
    
    BOOST_FOREACH (VSC::OB::InputListener::WPtr inputListener, this->getInputListeners())
    {
        InputListener::SPtr sinputListener = inputListener.lock();
        if (sinputListener) {
            sinputListener->mouseButtonPressed(renderWindow, position, buttonID);
        }
    }
}

void VSC::OB::InputAdapter::mouseButtonReleased(Ogre::RenderWindow* renderWindow, const Ogre::Vector2& position, OIS::MouseButtonID buttonID)
{
    mCurrentMouseButtons.erase(buttonID);
    
    mLastMousePosition = position;
    
    BOOST_FOREACH (VSC::OB::InputListener::WPtr inputListener, this->getInputListeners())
    {
        InputListener::SPtr sinputListener = inputListener.lock();
        if (sinputListener) {
            sinputListener->mouseButtonReleased(renderWindow, position, buttonID);
        }
    }
}

void VSC::OB::InputAdapter::keyPressed(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
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
        BOOST_FOREACH (VSC::OB::InputListener::WPtr inputListener, this->getInputListeners())
        {
            InputListener::SPtr sinputListener = inputListener.lock();
            if (sinputListener) {
                sinputListener->keyPressed(renderWindow, key);
            }
        }
    }

}

void VSC::OB::InputAdapter::keyReleased(Ogre::RenderWindow* renderWindow, OIS::KeyCode key)
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
        BOOST_FOREACH (VSC::OB::InputListener::WPtr inputListener, this->getInputListeners())
        {
            InputListener::SPtr sinputListener = inputListener.lock();
            if (sinputListener) {
                sinputListener->keyReleased(renderWindow, key);
            }
        }
    }
}

void VSC::OB::InputAdapter::modifierChanged(Ogre::RenderWindow* renderWindow, OIS::Keyboard::Modifier modifier)
{
    mCurrentModifier = modifier;
    
    BOOST_FOREACH (VSC::OB::InputListener::WPtr inputListener, this->getInputListeners())
    {
        InputListener::SPtr sinputListener = inputListener.lock();
        if (sinputListener) {
            sinputListener->modifierChanged(renderWindow, modifier);
        }
    }
    
}

void VSC::OB::InputAdapter::renderWindowChangedSize(Ogre::RenderWindow* renderWindow)
{
    
    BOOST_FOREACH (VSC::OB::InputListener::WPtr inputListener, this->getInputListeners())
    {
        InputListener::SPtr sinputListener = inputListener.lock();
        if (sinputListener) {
            sinputListener->renderWindowChangedSize(renderWindow);
        }
    }
}


