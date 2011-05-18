//
//  VSCKeyboard.h
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 5/18/11.
//  Copyright 2011 NXP. All rights reserved.
//

#ifndef VSC_KEYBOARD_H
#define VSC_KEYBOARD_H


#ifdef __APPLE__
#import <Cocoa/Cocoa.h>
#define VSCUpArrowFunctionKey       NSUpArrowFunctionKey
#define VSCDownArrowFunctionKey     NSDownArrowFunctionKey
#define VSCLeftArrowFunctionKey     NSLeftArrowFunctionKey
#define VSCRightArrowFunctionKey    NSRightArrowFunctionKey
#else
#define VSCUpArrowFunctionKey       NULL
#define VSCDownArrowFunctionKey     NULL
#define VSCLeftArrowFunctionKey     NULL
#define VSCRightArrowFunctionKey    NULL
#endif

#endif // VSC_KEYBOARD_H

