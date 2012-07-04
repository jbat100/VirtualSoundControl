/***************************************************************************
This source file is part of OGREBULLET
(Object-oriented Graphics Rendering Engine Bullet Wrapper)
For the latest info, see http://www.ogre3d.org/phpBB2addons/viewforum.php?f=10
Copyright (c) 2007 tuan.kuranes@gmail.com (Use it Freely, even Statically, but have to contribute any changes)
This source file is not LGPL, it's public source code that you can reuse.
-----------------------------------------------------------------------------*/
/***************************************************************************
File modified for VSC project
 -----------------------------------------------------------------------------*/

#include "VSCOgreBulletCocoaInputAdapter.h"
#include "VSCOgreBulletListener.h"
#include "VSCOgreBulletCocoaInputListener.h"
#include "OgreOSXCocoaView.h"

#include <boost/foreach.hpp>

using namespace OIS;

VSCOgreBulletCocoaInputAdapter::VSCOgreBulletCocoaInputAdapter(OgreView *cocoaOgreView) :
mCocoaOgreView(cocoaOgreView)
{
    
}

void VSCOgreBulletCocoaInputAdapter::populateKeyConversionMap(void) {
    
    // Virtual Key Map to KeyCode
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x12, KC_1));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x13, KC_2));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x14, KC_3));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x15, KC_4));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x17, KC_5));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x16, KC_6));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x1A, KC_7));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x1C, KC_8));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x19, KC_9));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x1D, KC_0));
	
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x33, KC_BACK));  // might be wrong
	
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x1B, KC_MINUS));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x18, KC_EQUALS));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x31, KC_SPACE));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x2B, KC_COMMA));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x2F, KC_PERIOD));
	
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x2A, KC_BACKSLASH));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x2C, KC_SLASH));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x21, KC_LBRACKET));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x1E, KC_RBRACKET));
	
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x35, KC_ESCAPE));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x39, KC_CAPITAL));
	
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x30, KC_TAB));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x24, KC_RETURN));  // double check return/enter
	
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x29, KC_SEMICOLON));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x27, KC_APOSTROPHE));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x32, KC_GRAVE));
	
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x0B, KC_B));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x00, KC_A));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x08, KC_C));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x02, KC_D));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x0E, KC_E));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x03, KC_F));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x05, KC_G));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x04, KC_H));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x22, KC_I));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x26, KC_J));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x28, KC_K));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x25, KC_L));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x2E, KC_M));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x2D, KC_N));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x1F, KC_O));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x23, KC_P));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x0C, KC_Q));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x0F, KC_R));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x01, KC_S));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x11, KC_T));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x20, KC_U));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x09, KC_V));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x0D, KC_W));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x07, KC_X));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x10, KC_Y));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x06, KC_Z));
	
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x7A, KC_F1));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x78, KC_F2));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x63, KC_F3));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x76, KC_F4));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x60, KC_F5));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x61, KC_F6));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x62, KC_F7));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x64, KC_F8));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x65, KC_F9));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x6D, KC_F10));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x67, KC_F11));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x6F, KC_F12));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x69, KC_F13));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x6B, KC_F14));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x71, KC_F15));
	
	//Keypad
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x52, KC_NUMPAD0));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x53, KC_NUMPAD1));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x54, KC_NUMPAD2));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x55, KC_NUMPAD3));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x56, KC_NUMPAD4));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x57, KC_NUMPAD5));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x58, KC_NUMPAD6));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x59, KC_NUMPAD7));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x5B, KC_NUMPAD8));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x5C, KC_NUMPAD9));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x45, KC_ADD));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x4E, KC_SUBTRACT));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x41, KC_DECIMAL));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x51, KC_NUMPADEQUALS));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x4B, KC_DIVIDE));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x43, KC_MULTIPLY));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x4C, KC_NUMPADENTER));
	
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x7E, KC_UP));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x7D, KC_DOWN));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x7B, KC_LEFT));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x7C, KC_RIGHT));
	
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x74, KC_PGUP));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x79, KC_PGDOWN));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x73, KC_HOME));
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x77, KC_END));
	
	keyConversionMap.insert(UnicarToOISKeyMap::value_type(0x75, KC_DELETE)); 
    
}


std::set<OIS::KeyCode> VSCOgreBulletCocoaInputAdapter::keyCodesForCocoaString(NSString* s) {
    
    std::set<OIS::KeyCode> keyCodeSet;
    
    for (NSUInteger i = 0; i < [s length]; i++)
    {
        UInt32 c = (UInt32)[s characterAtIndex:i];
        UnicarToOISKeyMap::iterator it =  keyConversionMap.find(c);
        if (it != keyConversionMap.end()) {
            keyCodeSet.insert(it->second);
        }
    }
    
    return keyCodeSet;
    
}

OIS::Keyboard::Modifier VSCOgreBulletCocoaInputAdapter::modiferForCocoaModifierFlags(NSUInteger m) {
    
    OIS::Keyboard::Modifier oisMod = (OIS::Keyboard::Modifier)0;
    
    if (m && NSShiftKeyMask)        oisMod = (OIS::Keyboard::Modifier) (oisMod | OIS::Keyboard::Shift);
    if (m && NSControlKeyMask)      oisMod = (OIS::Keyboard::Modifier) (oisMod | OIS::Keyboard::Ctrl);
    if (m && NSAlternateKeyMask)    oisMod = (OIS::Keyboard::Modifier) (oisMod | OIS::Keyboard::Alt);
    
    return oisMod;
    
}

void VSCOgreBulletCocoaInputAdapter::keyUp(NSEvent* theEvent) {
    
    NSString* keys = [theEvent charactersIgnoringModifiers];
    
    std::set<OIS::KeyCode> keyCodeSet = this->keyCodesForCocoaString(keys);

    BOOST_FOREACH( OIS::KeyCode key, keyCodeSet )
    {
        OIS::KeyEvent e(NULL, key, 0);
        bool b = mOgreBulletInputListener->keyReleased(e);
    }    
}

void VSCOgreBulletCocoaInputAdapter::keyDown(NSEvent* theEvent) {
    
    NSString* keys = [theEvent charactersIgnoringModifiers];
    
    std::set<OIS::KeyCode> keyCodeSet = this->keyCodesForCocoaString(keys);
    
    BOOST_FOREACH( OIS::KeyCode key, keyCodeSet )
    {
        OIS::KeyEvent e(NULL, key, 0);
        bool b = mOgreBulletInputListener->keyPressed(e);
    }  
    
}

void VSCOgreBulletCocoaInputAdapter::mouseEntered(NSEvent* theEvent) {
    
    
    
}

void VSCOgreBulletCocoaInputAdapter::mouseExited(NSEvent* theEvent) {
    
}

void VSCOgreBulletCocoaInputAdapter::mouseMoved(NSEvent* theEvent) {
    
}


void VSCOgreBulletCocoaInputAdapter::mouseDown(NSEvent* theEvent) {
    
}

void VSCOgreBulletCocoaInputAdapter::mouseUp(NSEvent* theEvent) {
    
}

void VSCOgreBulletCocoaInputAdapter::mouseDragged(NSEvent* theEvent) {
    
}


void VSCOgreBulletCocoaInputAdapter::otherMouseDown(NSEvent* theEvent) {
    
}

void VSCOgreBulletCocoaInputAdapter::otherMouseDragged(NSEvent* theEvent) {
    
}

void VSCOgreBulletCocoaInputAdapter::otherMouseUp(NSEvent* theEvent) {
    
}


void VSCOgreBulletCocoaInputAdapter::rightMouseDown(NSEvent* theEvent) {
    
}

void VSCOgreBulletCocoaInputAdapter::rightMouseDragged(NSEvent* theEvent) {
    
}

void VSCOgreBulletCocoaInputAdapter::rightMouseUp(NSEvent* theEvent) {
    
}


void VSCOgreBulletCocoaInputAdapter::scrollWheel(NSEvent* theEvent) {
    
}


