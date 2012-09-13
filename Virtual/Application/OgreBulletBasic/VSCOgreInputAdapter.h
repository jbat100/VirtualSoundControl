
#ifndef _VSC_OGRE_BULLET_INPUT_ADAPTER_H_
#define _VSC_OGRE_BULLET_INPUT_ADAPTER_H_

#include "VSCUI.h"
#include "OIS.h"
#include <set>
#include <Ogre/Ogre.h>

class VSCOgreInputListener;

class VSCOgreInputAdapter
{
    
public:
    
    VSCOgreInputAdapter(void);
    virtual ~VSCOgreInputAdapter(void) {}
    
    //friend class VSCOgreInputListener;
    
    /*
     *  Add/Remove input listeners
     */
    
    const std::set<VSCOgreInputListener*>& getInputListeners(void) {return mInputListeners;}
    void addInputListener(VSCOgreInputListener* listener);
    void removeInputListener(VSCOgreInputListener* listener);
    
    bool areCoordinatesNormalized(void) {return mNormalizedCoordinates;}
    void normalizeCoordinates(bool norm) {mNormalizedCoordinates = norm;}
    
    bool areRapidFireKeyPressesAllowed(void) {return mAllowRapidFireKeyPresses;}
    void allowRapidFireKeyPresses(bool norm) {mAllowRapidFireKeyPresses = norm;}
    
    /*
     *  Listener Keyboard stuff query 
     */
    
    const VSCKeyCodeSet&  getCurrentKeys() {return mCurrentKeys;} // currently pressed non modifier keys
    bool isKeyPressed(OIS::KeyCode key); 
    OIS::Keyboard::Modifier getCurrentModifier(void); // OIS::Keyboard::Modifier is a bit mask
    
    /*
     *  Listener Mouse stuff query 
     */
    
    bool isMouseButtonPressed(OIS::MouseButtonID) const;
    Ogre::Vector2 getLastMousePosition() const {return mLastMousePosition;}
    Ogre::Vector2 getLastMouseMovement() const {return mLastMouseMovement;}
    Ogre::Vector2 getBufferedMouseMovement() const {return mBufferedMouseMovement;}
    void resetBufferedMouseMovement() {mBufferedMouseMovement = 0;}
    
protected:
    
    /*
     *  Called by platform specific subclasses to translate from their input detection
     *  mechanisms, will forward calls to the listeners.
     */
    
    void mouseMoved(const Ogre::Vector2& position, const Ogre::Vector2& movement);
    void mouseEntered(const Ogre::Vector2& position);
    void mouseExited(const Ogre::Vector2& position);
    void mouseButtonPressed(const Ogre::Vector2& position, OIS::MouseButtonID buttonID);
    void mouseButtonReleased(const Ogre::Vector2& position, OIS::MouseButtonID buttonID);
    void keyPressed(OIS::KeyCode key);
    void keyReleased(OIS::KeyCode key);
    void modifierChanged(OIS::Keyboard::Modifier modifier);
    
private:

    std::set<VSCOgreInputListener*>     mInputListeners;
    bool                                mNormalizedCoordinates;
    bool                                mAllowRapidFireKeyPresses;
    
    Ogre::Vector2                       mLastMousePosition;
    Ogre::Vector2                       mLastMouseMovement;
    Ogre::Vector2                       mBufferedMouseMovement;
    
    VSCKeyCodeSet                       mCurrentKeys;
    VSCMouseButtonSet                   mCurrentMouseButtons;
    
    OIS::Keyboard::Modifier             mCurrentModifier;
    
};

#endif//_VSC_OGRE_BULLET_INPUT_ADAPTER_H_

