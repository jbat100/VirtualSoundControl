
#ifndef _VSC_OGRE_BULLET_INPUT_RESPONDER_H_
#define _VSC_OGRE_BULLET_INPUT_RESPONDER_H_

#include <set>
#include <Ogre/Ogre.h>
#include "OIS.h"
class VSCOgreInputListener;

class VSCOgreInputResponder
{
    
public:
    
    VSCOgreInputResponder(); 
    virtual VSCOgreInputResponder(){}
    
    /*----------------------------------------------------------------
     *  This is the interface which I think should be used
     */
    VSCOgreInputListener*   getInputListener(void) {return mInputListener;}
    VSCOgreInputResponder*  getNextInputResponder(void) {return mNextInputResponder;}
    void                    setNextInputResponder(VSCOgreInputResponder* next);
    
    /**--------------------------------------------------------------
     *  These methods are called by the input adapters when some 
     *  input related thing happens, then chained down the responders. 
     *  Do not call these methods 
     *  in any other case, unless you want to simulate input.
     *
     *  The decision to not make these methods purely virtual is deliberate,
     *  so that subclasses can choose to implement the methods or not...
     */
    virtual bool mouseMoved(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position, const Ogre::Vector2& movement);
    virtual bool mouseEntered(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position);
    virtual bool mouseExited(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position);
    virtual bool mouseButtonPressed(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position, OIS::MouseButtonID buttonID);
    virtual bool mouseButtonReleased(VSCOgreInputAdapter* adapter, const Ogre::Vector2& position, OIS::MouseButtonID buttonID);
    virtual bool keyPressed(VSCOgreInputAdapter* adapter, OIS::KeyCode key);
    virtual bool keyReleased(VSCOgreInputAdapter* adapter, OIS::KeyCode key);
    
private:
    
    VSCOgreInputListener*       mInputListener;
    VSCOgreInputResponder*      mNextInputResponder;
};

#endif//_VSC_OGRE_BULLET_INPUT_LISTENER_H_

