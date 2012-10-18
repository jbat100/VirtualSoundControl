
#include "VSCInterfaceAdapter.h"
#include "VSCException.h"

#include <boost/assert.hpp>
#include <boost/foreach.hpp>


// MARK: - Interface Adapter

template<typename Context, typename Vector2>
VSC::InterfaceAdapter<Context, Vector2>::InterfaceAdapter(void) :
mCurrentModifier((OIS::Keyboard::Modifier)0),
mNormalizedCoordinates(false),
mLastMouseMovement(Vector2(0.0, 0.0)),
mLastMousePosition(Vector2(0.0, 0.0))
{
    
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::addResponder(typename Responder::SPtr responder)
{
    BOOST_ASSERT(responder);
    if (!responder) return;
    
    typename Responders::iterator it = std::find(mResponders.begin(), mResponders.end(), responder);
    
    BOOST_ASSERT(it == mResponders.end());
    if (it != mResponders.end()) return;
    else mResponders.push_back(responder);
    
    responder->setInterfaceAdapter(InterfaceAdapter<Context, Vector2>::shared_from_this());
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::removeResponder(typename Responder::SPtr responder)
{
    BOOST_ASSERT(responder);
    if (!responder) return;
    
    typename Responders::iterator it = std::find(mResponders.begin(), mResponders.end(), responder);
    
    if (it != mResponders.end()) mResponders.erase(it);
    
    responder->setInterfaceAdapter(InterfaceAdapter::SPtr());
}

template<typename Context, typename Vector2>
bool VSC::InterfaceAdapter<Context, Vector2>::isKeyPressed(OIS::KeyCode key)
{
    return (mCurrentKeys.find(key) != mCurrentKeys.end());
}

template<typename Context, typename Vector2>
bool VSC::InterfaceAdapter<Context, Vector2>::isMouseButtonPressed(OIS::MouseButtonID buttonid) const
{
    return (mCurrentMouseButtons.find(buttonid) != mCurrentMouseButtons.end());
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::contextChanged(const Context* context)
{
    BOOST_FOREACH (typename Responder::WPtr responder, this->getResponders())
    {
        typename Responder::SPtr sresponder = responder.lock();
        if (sresponder) {
            sresponder->contextChanged(context);
        }
    }
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::mouseMoved(const Context* context,
                                                         const Vector2& position,
                                                         const Vector2& movement)
{
    mLastMousePosition = position;
    mLastMouseMovement = movement;
    mBufferedMouseMovement += movement;
    
    BOOST_FOREACH (typename Responder::WPtr responder, this->getResponders())
    {
        typename Responder::SPtr sresponder = responder.lock();
        if (sresponder) {
            sresponder->mouseMoved(context, position, movement);
        }
    }
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::mouseEntered(const Context* context,
                                                           const Vector2& position)
{
    
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::mouseExited(const Context* context,
                                                          const Vector2& position)
{
    
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::mouseButtonPressed(const Context* context,
                                                                 const Vector2& position,
                                                                 OIS::MouseButtonID buttonID)
{
    mCurrentMouseButtons.insert(buttonID);
    
    mLastMousePosition = position;
    
    BOOST_FOREACH (typename Responder::WPtr responder, this->getResponders())
    {
        typename Responder::SPtr sresponder = responder.lock();
        if (sresponder) {
            sresponder->mouseButtonPressed(context, position, buttonID);
        }
    }
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::mouseButtonReleased(const Context* context,
                                                                  const Vector2& position,
                                                                  OIS::MouseButtonID buttonID)
{
    mCurrentMouseButtons.erase(buttonID);
    
    mLastMousePosition = position;
    
    BOOST_FOREACH (typename Responder::WPtr responder, this->getResponders())
    {
        typename Responder::SPtr sresponder = responder.lock();
        if (sresponder) {
            sresponder->mouseButtonReleased(context, position, buttonID);
        }
    }
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::keyPressed(const Context* context, OIS::KeyCode key)
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
        BOOST_FOREACH (typename Responder::WPtr responder, this->getResponders())
        {
            typename Responder::SPtr sresponder = responder.lock();
            if (sresponder) {
                sresponder->keyPressed(context, key);
            }
        }
    }

}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::keyReleased(const Context* context, OIS::KeyCode key)
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
        BOOST_FOREACH (typename Responder::WPtr responder, this->getResponders())
        {
            typename Responder::SPtr sresponder = responder.lock();
            if (sresponder) {
                sresponder->keyReleased(context, key);
            }
        }
    }
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::modifierChanged(const Context* context, OIS::Keyboard::Modifier modifier)
{
    mCurrentModifier = modifier;
    
    BOOST_FOREACH (typename Responder::WPtr responder, this->getResponders())
    {
        typename Responder::SPtr sresponder = responder.lock();
        if (sresponder) {
            sresponder->modifierChanged(context, modifier);
        }
    }
    
}


// MARK: - Responder


template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::Responder::setInterfaceAdapter(InterfaceAdapter::SPtr adapter)
{
    mAdapter = InterfaceAdapter::WPtr(adapter);

    if (this->getNextResponder())
    {
        this->getNextResponder()->setInputAdapter(mAdapter);
    }
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::Responder::setNextResponder(Responder::SPtr next)
{
    mNextResponder = Responder::WPtr(next);
    
    if (this->getNextResponder())
    {
        this->getNextResponder()->setInputAdapter(mAdapter);
    }
}

template<typename Context, typename Vector2>
bool VSC::InterfaceAdapter<Context, Vector2>::Responder::contextChanged(const Context* context)
{
    if (this->getNextResponder())
    {
        return this->getNextResponder()->contextChanged(context);
    }
        
    return false;
}

template<typename Context, typename Vector2>
bool VSC::InterfaceAdapter<Context, Vector2>::Responder::mouseMoved(const Context* context,
                                                                    const Vector2& position,
                                                                    const Vector2& movement)
{
    if (mTraceUI) std::cout << "VSC::OB::InputListener::mouseMoved, next listener " << std::endl;
    if (this->getNextResponder())
    {
        return this->getNextResponder()->mouseMoved(context, position, movement);
    }
    
    return false;
}

template<typename Context, typename Vector2>
bool VSC::InterfaceAdapter<Context, Vector2>::Responder::mouseEntered(const Context* context,
                                                                      const Vector2& position)
{
    if (this->getNextResponder())
    {
        return this->getNextResponder()->mouseEntered(context, position);
    }
    
    return false;
}

template<typename Context, typename Vector2>
bool VSC::InterfaceAdapter<Context, Vector2>::Responder::mouseExited(const Context* context,
                                                                     const Vector2& position)
{
    if (this->getNextResponder())
    {
        return this->getNextResponder()->mouseEntered(context, position);
    }
    
    return false;
}

template<typename Context, typename Vector2>
bool VSC::InterfaceAdapter<Context, Vector2>::Responder::mouseButtonPressed(const Context* context,
                                                                            const Vector2& position,
                                                                            OIS::MouseButtonID buttonID)
{
    if (mTraceUI) std::cout << "VSC::OB::InputListener::mouseButtonPressed, next listener" << std::endl;
    if (this->getNextResponder())
    {
        return this->getNextResponder()->mouseButtonPressed(context, position, buttonID);
    }
    
    return false;
}

template<typename Context, typename Vector2>
bool VSC::InterfaceAdapter<Context, Vector2>::Responder::mouseButtonReleased(const Context* context,
                                                                             const Vector2& position,
                                                                             OIS::MouseButtonID buttonID)
{
    if (mTraceUI) std::cout << "VSC::OB::InputListener::mouseButtonReleased, next listener " << std::endl;
    if (this->getNextResponder())
    {
        return this->getNextResponder()->mouseButtonReleased(context, position, buttonID);
    }
    
    return false;
}

template<typename Context, typename Vector2>
bool VSC::InterfaceAdapter<Context, Vector2>::Responder::keyPressed(const Context* context, OIS::KeyCode key)
{
    if (mTraceUI) std::cout << "VSC::OB::InputListener::keyPressed " << key << std::endl;
    if (this->getNextResponder())
    {
        return this->getNextResponder()->keyPressed(context, key);
    }
    
    return false;
}

template<typename Context, typename Vector2>
bool VSC::InterfaceAdapter<Context, Vector2>::Responder::keyReleased(const Context* context, OIS::KeyCode key)
{
    if (this->getNextResponder())
    {
        return this->getNextResponder()->keyReleased(context, key);
    }
    
    return false;
}

template<typename Context, typename Vector2>
bool VSC::InterfaceAdapter<Context, Vector2>::Responder::modifierChanged(const Context* context,
                                                                         OIS::Keyboard::Modifier modifier)
{
    if (this->getNextResponder())
    {
        return this->getNextResponder()->modifierChanged(context, modifier);
    }
    
    return false;
}


// MARK: ResponderChain

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::ResponderChain::appendResponder(typename Responder::SPtr responder)
{
    typename Responders::iterator it = mResponders.end();
    this->insertResponder(responder, it);
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::ResponderChain::prependResponder(typename Responder::SPtr responder)
{
    typename Responders::iterator it = mResponders.begin();
    this->insertResponder(responder, it);
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::ResponderChain::insertResponderAfterResponder(typename Responder::SPtr newResponder,
                                                                                            typename Responder::SPtr responder)
{
    BOOST_ASSERT(responder);
    
    if (responder) {
        typename Responders::const_iterator it = std::find(mResponders.begin(), mResponders.end(), responder);
        BOOST_ASSERT(it != mResponders.end());
        if (it != mResponders.end()) this->insertResponder(responder, ++it);
    }
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::ResponderChain::insertResponderBeforeResponder(typename Responder::SPtr newResponder,
                                                                                             typename Responder::SPtr responder)
{
    BOOST_ASSERT(responder);
    
    if (responder) {
        typename Responders::const_iterator it = std::find(mResponders.begin(), mResponders.end(), responder);
        BOOST_ASSERT(it != mResponders.end());
        if (it != mResponders.end()) this->insertResponder(responder, it);
    }
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::ResponderChain::removeResponder(typename Responder::SPtr responder)
{
    BOOST_ASSERT(responder);
    
    if (responder) {
        typename Responders::const_iterator it = std::find(mResponders.begin(), mResponders.end(), responder);
        BOOST_ASSERT(it != mResponders.end());
        if (it != mResponders.end()) mResponders.erase(it);
    }
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::ResponderChain::removeAllResponders(void)
{
    mResponders.clear();
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::ResponderChain::chainSceneControllers(void)
{
    if (mResponders.size() < 1) return;
    
    typename Responders::iterator it = mResponders.begin();
    typename Responders::iterator nextIt = it; ++nextIt;
    
    this->setNextResponder((*it).lock());
    
    while (nextIt != mResponders.end()) {
        typename Responder::SPtr responder = (*it)->lock();
        typename Responder::SPtr nextResponder = (*nextIt)->lock();
        responder->setNextResponder(nextResponder);
    }
}

template<typename Context, typename Vector2>
void VSC::InterfaceAdapter<Context, Vector2>::ResponderChain::insertResponder(typename Responder::SPtr responder,
                                                                              typename Responders::iterator it)
{
    BOOST_ASSERT(responder);
    
    if (this->containsResponder(responder))
        throw VSCInvalidArgumentException("Cannot insert a responder in a chain which already contains it");

    
    if (responder) {
        mResponders.insert(it, Responder::WPtr(responder));
        this->chainSceneControllers();
    }
}

template<typename Context, typename Vector2>
bool VSC::InterfaceAdapter<Context, Vector2>::ResponderChain::containsResponder(typename Responder::SPtr responder)
{
    BOOST_ASSERT(responder);
    
    if (responder) {
        typename Responders::const_iterator it = std::find(mResponders.begin(), mResponders.end(), responder);
        return it != mResponders.end();
    }
    
    return false;
    
}


