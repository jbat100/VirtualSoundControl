//
//  MeshGLView.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 24/05/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "BaseGLView.h"
#include <Carbon/Carbon.h>

@implementation BaseGLView


@synthesize mouseSensitivity, mouseInteractionEnabled, keyIntereactionEnabled;
@synthesize upIsPressed, downIsPressed, leftIsPressed, rightIsPressed, forwardIsPressed, backwardIsPressed;


- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
		
		mouseInteractionEnabled = YES;
		keyIntereactionEnabled = YES;
		
		mouseSensitivity = 1.0;
		
		upIsPressed = NO;
		downIsPressed = NO;
		leftIsPressed = NO;
		rightIsPressed = NO;
		forwardIsPressed = NO;
		backwardIsPressed = NO;
		
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}

- (void)mouseDragged:(NSEvent *)theEvent {
	
	if (mouseInteractionEnabled) {
		[self rotateCameraThetaBy:(mouseSensitivity*[theEvent deltaX])];
		[self rotateCameraPhiBy:(mouseSensitivity*[theEvent deltaY])];
		return;
	}
	
	[super mouseDragged:theEvent];
	
}

/*
 
 http://stackoverflow.com/questions/404830/cocoa-wont-capture-shift-modifier
 
 First, -charactersIgnoringModifiers doesn't ignore the shift key, so you will still get shifted characters 
 (i.e UPPERCASE and !%#$%^&*) returned from it. What's probably happening in your function is: You press shift-w, 
 your -isEqualTo: returns false because you're comparing a lowercase 'w' and an uppercase 'W', and so you return 
 before getting to the shift-detection code at the bottom. The simplest solution is to just check for both.
 
 However, if you want, for example, Arabic keyboardists to be able to easily use your app, you really shouldn't 
 hardcode characters that may not even appear on the user's keyboard. The value returned by -keyCode refers to a 
 key's position on the keyboard, not the represented character. For starters, the constants beginning in 'kVK_ANSI_' 
 and 'kVK_' in Events.h (you may have to link to Carbon.framework and #include <Carbon/Carbon.h> to use those 
 constants) can be compared to what -keyCode returns, and they refer to the key positions a QWERTY-using USian 
 expects. So you can be (pretty) sure that, regardless of keyboard layout, the keycodes for 'wasd' (kVK_ANSI_W, 
 kVK_ANSI_A, etc.) will refer to that triangle in the top left of your user's keyboard.
 
 */


- (void)keyDown:(NSEvent *)theEvent {
	

	/* 
	 
	 // This is done doing basic character check
	
	NSString *characters = [theEvent charactersIgnoringModifiers];
	
	unichar keyChar = 0;
	if ( [characters length] == 0 )
		return;            // reject dead keys
	
	if ( [characters length] == 1 ) {

		keyChar = [characters characterAtIndex:0];
		
		if ( keyChar == NSLeftArrowFunctionKey || keyChar == 'a') {
			leftIsPressed = YES;
			return;
		}
		
		if ( keyChar == NSRightArrowFunctionKey || keyChar == 'd') {
			rightIsPressed = YES;
			return;
		}
		
		if ( keyChar == NSUpArrowFunctionKey || keyChar == 'w') {
			forwardIsPressed = YES;
			return;
		}
		
		if ( keyChar == NSDownArrowFunctionKey || keyChar == 's') {
			backwardIsPressed = YES;
			return;
		}			
	}
	
	*/
	
	// This should be better as it should be able to handle any keyboard layout
	
	if ([theEvent modifierFlags]) {
		
		NSLog(@"There is at least one modeifier flag");
		
	}
	
	unsigned short code = [theEvent keyCode];
	
	if (code == kVK_ANSI_A || code == kVK_LeftArrow) {
		leftIsPressed = YES;
		return;
	}
	
	if (code == kVK_ANSI_W || code == kVK_UpArrow) {
		forwardIsPressed = YES;
		return;
	}
	
	if (code == kVK_ANSI_D || code == kVK_RightArrow) {
		rightIsPressed = YES;
		return;
	}
	
	if (code == kVK_ANSI_S || code == kVK_DownArrow) {
		backwardIsPressed = YES;
		return;
	}
		
	
    [super keyDown:theEvent];
	
}

- (void)keyUp:(NSEvent *)theEvent {
	
	/*
	
    if ([theEvent modifierFlags] & NSNumericPadKeyMask) { // arrow keys have this mask
		
        NSString *theArrow = [theEvent charactersIgnoringModifiers];
		
        unichar keyChar = 0;
        if ( [theArrow length] == 0 )
            return;            // reject dead keys
		
        if ( [theArrow length] == 1 ) {
			
            keyChar = [theArrow characterAtIndex:0];
			
            if ( keyChar == NSLeftArrowFunctionKey || keyChar == 'a') {
				leftIsPressed = NO;
                return;
            }
			
            if ( keyChar == NSRightArrowFunctionKey || keyChar == 'd') {
				rightIsPressed = NO;
                return;
            }
			
            if ( keyChar == NSUpArrowFunctionKey || keyChar == 'w') {
				forwardIsPressed = NO;
                return;
            }
			
            if ( keyChar == NSDownArrowFunctionKey || keyChar == 's') {
				backwardIsPressed = NO;
                return;
            }			
        }
		
    }
	 
	 */
	
	// This should be better as it should be able to handle any keyboard layout
	
	unsigned short code = [theEvent keyCode];

	
	if (code == kVK_ANSI_A || code == kVK_LeftArrow) {
		leftIsPressed = NO;
		return;
	}
	
	if (code == kVK_ANSI_W || code == kVK_UpArrow) {
		forwardIsPressed = NO;
		return;
	}
	
	if (code == kVK_ANSI_D || code == kVK_RightArrow) {
		rightIsPressed = NO;
		return;
	}
	
	if (code == kVK_ANSI_S || code == kVK_DownArrow) {
		backwardIsPressed = NO;
		return;
	}
	
	
	
    [super keyUp:theEvent];
	
}

-(void) rotateCameraThetaBy:(float)angle {
	
}

-(void) rotateCameraPhiBy:(float)angle {
	
}

@end
