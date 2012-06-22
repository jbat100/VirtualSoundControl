#ifndef _VSC_OGRE_BULLET_COCOA_INPUT_ADAPTER_H_
#define _VSC_OGRE_BULLET_COCOA_INPUT_ADAPTER_H_

#include "VSCOgreBulletInputListener.h"

class VSCOgreBulletCocoaInputListener;
@class NSEvent;
@class OgreView;

class VSCOgreBulletCocoaInputAdapter
{
public:

	// Constructor/destructor
    VSCOgreBulletCocoaInputAdapter(VSCOgreBulletCocoaInputListener *ogreBulletInputListener, OgreView *cocoaOgreView);
    virtual ~VSCOgreBulletCocoaInputAdapter(){};
    
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

private:

    VSCOgreBulletInputListener*     mOgreBulletInputListener;
    OgreView*                       mCocoaOgreView;
    
};

#endif//_VSC_OGRE_BULLET_COCOA_INPUT_ADAPTER_H_
