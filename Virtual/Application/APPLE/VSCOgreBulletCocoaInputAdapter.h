#ifndef _VSC_OGRE_BULLET_COCOA_INPUT_ADAPTER_H_
#define _VSC_OGRE_BULLET_COCOA_INPUT_ADAPTER_H_

#import <Cocoa/Cocoa.h>

//#include "VSCOgreBulletInputListener.h"
#include "OIS.h"
#include <set>

class VSCOgreBulletInputListener;
@class OgreView;

class VSCOgreBulletCocoaInputAdapter
{
public:

	// Constructor/destructor
    VSCOgreBulletCocoaInputAdapter(VSCOgreBulletInputListener *ogreBulletInputListener, OgreView *cocoaOgreView);
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
    
    void setOgreBulletInputListener(VSCOgreBulletInputListener* listener) {mOgreBulletInputListener = listener;}
    VSCOgreBulletInputListener* getOgreBulletInputListener(void) {return mOgreBulletInputListener;}
    
    void setOgreView(OgreView* ogreView) {mCocoaOgreView = ogreView;}
    OgreView* getOgreView(void) {return mCocoaOgreView;}

    std::set<OIS::KeyCode> keyCodesForCocoaString(NSString* s);
    OIS::Keyboard::Modifier modiferForCocoaModifierFlags(NSUInteger m);
    
private:
    
    typedef std::map<UInt32, OIS::KeyCode> UnicarToOISKeyMap;
    UnicarToOISKeyMap keyConversionMap;
    
    void populateKeyConversionMap(void);

    VSCOgreBulletInputListener*     mOgreBulletInputListener;
    OgreView*                       mCocoaOgreView;
    
};

#endif//_VSC_OGRE_BULLET_COCOA_INPUT_ADAPTER_H_
