
#include "VSCOgreBulletCocoaInputAdapter.h"
#include "VSCOgreBulletScene.h"
#include "VSCOgreBulletCocoaInputListener.h"
#include "OgreOSXCocoaView.h"

#include <boost/foreach.hpp>
#include <boost/assert.hpp>

using namespace OIS;

VSCOgreBulletCocoaInputAdapter::VSCOgreBulletCocoaInputAdapter(void) :
mCocoaView(0)
{
    this->populateKeyConversionMap();
}

Ogre::Vector2 VSCOgreBulletCocoaInputAdapter::adaptedMouseLocationForEvent(NSEvent* theEvent) {
    
    BOOST_ASSERT_MSG(mCocoaView, "Expected mCocoaView" ); 
    
    NSPoint location = [theEvent locationInWindow];
    NSPoint locationInView = [mCocoaView convertPoint:location fromView:nil];
    
    return Ogre::Vector2(locationInView.x,locationInView.y);
}

Ogre::Vector2 VSCOgreBulletCocoaInputAdapter::adaptedMouseMovementForEvent(NSEvent* theEvent) {
    return Ogre::Vector2([theEvent deltaX],[theEvent deltaY]);
}

OIS::MouseButtonID VSCOgreBulletCocoaInputAdapter::mouseButtonIdForEvent(NSEvent* theEvent) {
    
    NSInteger mouseButton = [theEvent buttonNumber];
    
    if (mTraceUI) std::cout << "VSCOgreBulletCocoaInputAdapter::mouseButtonIdForEvent, mouseButton is " << mouseButton << std::endl; 
    
    switch (mouseButton) {
        //case 1 << 0:
        case NSLeftMouseDown:
        case NSLeftMouseUp:
            return OIS::MB_Left;
        //case 1 << 1:
        case NSRightMouseDown:
        case NSRightMouseUp:
            return OIS::MB_Right;
        default:
            return OIS::MouseButtonID(mouseButton-1);
    }
    
    return OIS::MouseButtonID(mouseButton-1);
}

void VSCOgreBulletCocoaInputAdapter::populateKeyConversionMap(void) {
    
    //std::cout << "0x" << std::hex << this << " Populating keyConversionMap" << std::endl;
    
    // Virtual Key Map to KeyCode
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x12, KC_1));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x13, KC_2));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x14, KC_3));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x15, KC_4));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x17, KC_5));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x16, KC_6));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x1A, KC_7));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x1C, KC_8));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x19, KC_9));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x1D, KC_0));
	
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x33, KC_BACK));  // might be wrong
	
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x1B, KC_MINUS));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x18, KC_EQUALS));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x31, KC_SPACE));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x2B, KC_COMMA));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x2F, KC_PERIOD));
	
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x2A, KC_BACKSLASH));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x2C, KC_SLASH));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x21, KC_LBRACKET));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x1E, KC_RBRACKET));
	
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x35, KC_ESCAPE));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x39, KC_CAPITAL));
	
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x30, KC_TAB));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x24, KC_RETURN));  // double check return/enter
	
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x29, KC_SEMICOLON));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x27, KC_APOSTROPHE));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x32, KC_GRAVE));
	
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x0B, KC_B));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x00, KC_A));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x08, KC_C));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x02, KC_D));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x0E, KC_E));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x03, KC_F));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x05, KC_G));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x04, KC_H));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x22, KC_I));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x26, KC_J));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x28, KC_K));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x25, KC_L));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x2E, KC_M));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x2D, KC_N));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x1F, KC_O));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x23, KC_P));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x0C, KC_Q));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x0F, KC_R));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x01, KC_S));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x11, KC_T));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x20, KC_U));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x09, KC_V));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x0D, KC_W));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x07, KC_X));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x10, KC_Y));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x06, KC_Z));
	
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x7A, KC_F1));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x78, KC_F2));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x63, KC_F3));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x76, KC_F4));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x60, KC_F5));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x61, KC_F6));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x62, KC_F7));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x64, KC_F8));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x65, KC_F9));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x6D, KC_F10));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x67, KC_F11));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x6F, KC_F12));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x69, KC_F13));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x6B, KC_F14));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x71, KC_F15));
	
	//Keypad
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x52, KC_NUMPAD0));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x53, KC_NUMPAD1));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x54, KC_NUMPAD2));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x55, KC_NUMPAD3));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x56, KC_NUMPAD4));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x57, KC_NUMPAD5));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x58, KC_NUMPAD6));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x59, KC_NUMPAD7));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x5B, KC_NUMPAD8));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x5C, KC_NUMPAD9));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x45, KC_ADD));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x4E, KC_SUBTRACT));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x41, KC_DECIMAL));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x51, KC_NUMPADEQUALS));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x4B, KC_DIVIDE));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x43, KC_MULTIPLY));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x4C, KC_NUMPADENTER));
	
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x7E, KC_UP));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x7D, KC_DOWN));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x7B, KC_LEFT));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x7C, KC_RIGHT));
	
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x74, KC_PGUP));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x79, KC_PGDOWN));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x73, KC_HOME));
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x77, KC_END));
	
	keyConversionMap.insert(VirtualToOISKeyMap::value_type(0x75, KC_DELETE)); 
    
    //std::cout << "Populated keyConversionMap with " << keyConversionMap.size() << " elements" << std::endl;
    
    VirtualToOISKeyMap::iterator it1 =  keyConversionMap.find(0x77);
    VirtualToOISKeyMap::iterator it2 =  keyConversionMap.find(0x99);
    
}


OIS::KeyCode VSCOgreBulletCocoaInputAdapter::keyCodeForEvent(NSEvent* s) {
    
    unsigned short keyCode = (unsigned short)[s keyCode];
    if (mTrace) std::cout << "Finding ois for keyCode " << std::hex << keyCode << std::endl;
    VirtualToOISKeyMap::iterator it =  keyConversionMap.find(keyCode);
    if (it != keyConversionMap.end()) {
        if (mTrace) std::cout << "Found OIS::KeyCode " << std::hex << it->second << std::endl;
        return it->second;
    }
    if (mTrace) std::cout << "Unknown virtual key " << std::hex << keyCode << std::endl;
    
    return KC_UNASSIGNED;
    
}

OIS::Keyboard::Modifier VSCOgreBulletCocoaInputAdapter::modiferForCocoaModifierFlags(NSUInteger m) {
    
    OIS::Keyboard::Modifier oisMod = (OIS::Keyboard::Modifier)0;
    
    if (m && NSShiftKeyMask)        oisMod = (OIS::Keyboard::Modifier) (oisMod | OIS::Keyboard::Shift);
    if (m && NSControlKeyMask)      oisMod = (OIS::Keyboard::Modifier) (oisMod | OIS::Keyboard::Ctrl);
    if (m && NSAlternateKeyMask)    oisMod = (OIS::Keyboard::Modifier) (oisMod | OIS::Keyboard::Alt);
    
    return oisMod;
    
}

void VSCOgreBulletCocoaInputAdapter::keyUp(NSEvent* theEvent) {
    OIS::KeyCode key = this->keyCodeForEvent(theEvent);
    this->keyReleased(key);
}

void VSCOgreBulletCocoaInputAdapter::keyDown(NSEvent* theEvent) {
    OIS::KeyCode key = this->keyCodeForEvent(theEvent);
    this->keyPressed(key);
}

void VSCOgreBulletCocoaInputAdapter::mouseEntered(NSEvent* theEvent) {
    
    Ogre::Vector2 location = this->adaptedMouseLocationForEvent(theEvent);
    Ogre::Vector2 movement = this->adaptedMouseMovementForEvent(theEvent);
    
    
}

void VSCOgreBulletCocoaInputAdapter::mouseExited(NSEvent* theEvent) {
    
}

void VSCOgreBulletCocoaInputAdapter::mouseMoved(NSEvent* theEvent) {
    
    Ogre::Vector2 location = this->adaptedMouseLocationForEvent(theEvent);
    Ogre::Vector2 movement = this->adaptedMouseMovementForEvent(theEvent);
    
    VSCOgreInputAdapter::mouseMoved(location, movement);
    
}


void VSCOgreBulletCocoaInputAdapter::mouseDown(NSEvent* theEvent) {
    
    Ogre::Vector2 location = this->adaptedMouseLocationForEvent(theEvent);
    
    //OIS::MouseButtonID mouseButtonID = this->mouseButtonIdForEvent(theEvent);
    OIS::MouseButtonID mouseButtonID = OIS::MB_Left;
    
    this->mouseButtonPressed(location, mouseButtonID);
    
}

void VSCOgreBulletCocoaInputAdapter::mouseUp(NSEvent* theEvent) {
    
    Ogre::Vector2 location = this->adaptedMouseLocationForEvent(theEvent);
    
    //OIS::MouseButtonID mouseButtonID = this->mouseButtonIdForEvent(theEvent);
    OIS::MouseButtonID mouseButtonID = OIS::MB_Left;
    
    this->mouseButtonReleased(location, mouseButtonID);
    
}

void VSCOgreBulletCocoaInputAdapter::mouseDragged(NSEvent* theEvent) {
    
    Ogre::Vector2 location = this->adaptedMouseLocationForEvent(theEvent);
    Ogre::Vector2 movement = this->adaptedMouseMovementForEvent(theEvent);
    
    VSCOgreInputAdapter::mouseMoved(location, movement);
    
}


void VSCOgreBulletCocoaInputAdapter::otherMouseDown(NSEvent* theEvent) {
    //this->mouseDown(theEvent);
    Ogre::Vector2 location = this->adaptedMouseLocationForEvent(theEvent);
    //OIS::MouseButtonID mouseButtonID = this->mouseButtonIdForEvent(theEvent);
    OIS::MouseButtonID mouseButtonID = OIS::MB_Middle;
    this->mouseButtonPressed(location, mouseButtonID);
}

void VSCOgreBulletCocoaInputAdapter::otherMouseDragged(NSEvent* theEvent) {
    this->mouseMoved(theEvent);
}

void VSCOgreBulletCocoaInputAdapter::otherMouseUp(NSEvent* theEvent) {
    //this->mouseUp(theEvent);
    Ogre::Vector2 location = this->adaptedMouseLocationForEvent(theEvent);
    //OIS::MouseButtonID mouseButtonID = this->mouseButtonIdForEvent(theEvent);
    OIS::MouseButtonID mouseButtonID = OIS::MB_Middle;
    this->mouseButtonReleased(location, mouseButtonID);
}


void VSCOgreBulletCocoaInputAdapter::rightMouseDown(NSEvent* theEvent) {
    //this->mouseDown(theEvent);
    Ogre::Vector2 location = this->adaptedMouseLocationForEvent(theEvent);
    //OIS::MouseButtonID mouseButtonID = this->mouseButtonIdForEvent(theEvent);
    OIS::MouseButtonID mouseButtonID = OIS::MB_Right;
    this->mouseButtonPressed(location, mouseButtonID);
}

void VSCOgreBulletCocoaInputAdapter::rightMouseDragged(NSEvent* theEvent) {
    this->mouseMoved(theEvent);
}

void VSCOgreBulletCocoaInputAdapter::rightMouseUp(NSEvent* theEvent) {
    //this->mouseUp(theEvent);
    Ogre::Vector2 location = this->adaptedMouseLocationForEvent(theEvent);
    //OIS::MouseButtonID mouseButtonID = this->mouseButtonIdForEvent(theEvent);
    OIS::MouseButtonID mouseButtonID = OIS::MB_Right;
    this->mouseButtonReleased(location, mouseButtonID);
}


void VSCOgreBulletCocoaInputAdapter::scrollWheel(NSEvent* theEvent) {
    
}


