#ifndef _VSC_OGRE_BULLET_COCOA_INPUT_ADAPTER_H_
#define _VSC_OGRE_BULLET_COCOA_INPUT_ADAPTER_H_

#import <Cocoa/Cocoa.h>
#include <Ogre/Ogre.h>
#include "OIS.h"
#include "VSCOgreInputAdapter.h"
#include <set>

class VSCOgreInputListener;

class VSCOgreBulletCocoaInputAdapter : public VSCOgreInputAdapter
{
public:
    
    VSCOgreBulletCocoaInputAdapter(void);
    
    /*
     *  Slots for forwarding the appropriate NSResponder callbacks
     */
    void keyUp(NSEvent* theEvent);
    void keyDown(NSEvent* theEvent);
    void mouseEntered(NSEvent* theEvent);
    void mouseExited(NSEvent* theEvent);
    void mouseMoved(NSEvent* theEvent);
    void mouseDown(NSEvent* theEvent);
    void mouseUp(NSEvent* theEvent);
    void mouseDragged(NSEvent* theEvent);
    void otherMouseDown(NSEvent* theEvent);
    void otherMouseDragged(NSEvent* theEvent);
    void otherMouseUp(NSEvent* theEvent);
    void rightMouseDown(NSEvent* theEvent);
    void rightMouseDragged(NSEvent* theEvent);
    void rightMouseUp(NSEvent* theEvent);
    void scrollWheel(NSEvent* theEvent);
    

    /*
     *  OIS/Cocoa conversion utilities
     */
    OIS::KeyCode keyCodeForEvent(NSEvent* e);
    OIS::Keyboard::Modifier modiferForCocoaModifierFlags(NSUInteger m);
    
    /*
     *  Cocoa view
     */
    void setCocoaView(NSView* v){mCocoaView = v;}
    NSView* getCocoaView(void){return mCocoaView;}
    
private:
    
    Ogre::Vector2 adaptedMouseLocationForEvent(NSEvent* theEvent);
    Ogre::Vector2 adaptedMouseMovementForEvent(NSEvent* theEvent);
    OIS::MouseButtonID mouseButtonIdForEvent(NSEvent* theEvent);
    
    typedef std::map<unsigned short, OIS::KeyCode> VirtualToOISKeyMap;
    VirtualToOISKeyMap keyConversionMap;
    void populateKeyConversionMap(void);
    
    NSView* mCocoaView;
    
    static const bool mTrace = false;
    static const bool mTraceUI = true;
    
};

#endif//_VSC_OGRE_BULLET_COCOA_INPUT_ADAPTER_H_
