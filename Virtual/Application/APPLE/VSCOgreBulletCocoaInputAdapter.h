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
    
    void keyUp(Ogre::RenderWindow* renderWindow, NSEvent* theEvent);
    void keyDown(Ogre::RenderWindow* renderWindow, NSEvent* theEvent);
    
    void mouseEntered(Ogre::RenderWindow* renderWindow, NSEvent* theEvent);
    void mouseExited(Ogre::RenderWindow* renderWindow, NSEvent* theEvent);
    void mouseMoved(Ogre::RenderWindow* renderWindow, NSEvent* theEvent);
    void mouseDown(Ogre::RenderWindow* renderWindow, NSEvent* theEvent);
    void mouseUp(Ogre::RenderWindow* renderWindow, NSEvent* theEvent);
    void mouseDragged(Ogre::RenderWindow* renderWindow, NSEvent* theEvent);
    void otherMouseDown(Ogre::RenderWindow* renderWindow, NSEvent* theEvent);
    void otherMouseDragged(Ogre::RenderWindow* renderWindow, NSEvent* theEvent);
    void otherMouseUp(Ogre::RenderWindow* renderWindow, NSEvent* theEvent);
    void rightMouseDown(Ogre::RenderWindow* renderWindow, NSEvent* theEvent);
    void rightMouseDragged(Ogre::RenderWindow* renderWindow, NSEvent* theEvent);
    void rightMouseUp(Ogre::RenderWindow* renderWindow, NSEvent* theEvent);
    void scrollWheel(Ogre::RenderWindow* renderWindow, NSEvent* theEvent);
    

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
