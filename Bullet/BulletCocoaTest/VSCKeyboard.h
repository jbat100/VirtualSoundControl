//
//  VSCKeyboard.h
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 5/18/11.
//  Copyright 2011 NXP. All rights reserved.
//

#ifndef VSC_KEYBOARD_H
#define VSC_KEYBOARD_H


#ifdef __APPLE__ // IF APPLE
#import <Cocoa/Cocoa.h>
// Special Keys
#define VSCUpArrowFunctionKey       NSUpArrowFunctionKey
#define VSCDownArrowFunctionKey     NSDownArrowFunctionKey
#define VSCLeftArrowFunctionKey     NSLeftArrowFunctionKey
#define VSCRightArrowFunctionKey    NSRightArrowFunctionKey
// Modifier masks
#define VSCAlphaShiftKeyMask		NSAlphaShiftKeyMask
#define VSCShiftKeyMask				NSShiftKeyMask
#define VSCControlKeyMask			NSControlKeyMask
#define VSCAlternateKeyMask			NSAlternateKeyMask
#define VSCCommandKeyMask			NSCommandKeyMask
#else // IF OTHER THAN APPLE
// Special Keys
#define VSCUpArrowFunctionKey       NULL
#define VSCDownArrowFunctionKey     NULL
#define VSCLeftArrowFunctionKey     NULL
#define VSCRightArrowFunctionKey    NULL
// Modifier masks
#define VSCAlphaShiftKeyMask		0
#define VSCShiftKeyMask				0
#define VSCControlKeyMask			0
#define VSCAlternateKeyMask			0
#define VSCCommandKeyMask			0
#endif

// Just the mofifiers that VSC cares about...
#define VSCModifierMask VSCAlphaShiftKeyMask|VSCShiftKeyMask|VSCControlKeyMask|VSCAlternateKeyMask|VSCCommandKeyMask

#endif // VSC_KEYBOARD_H

