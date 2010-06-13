//
//  MeshGLView.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 24/05/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "BaseGLView.h"
#include <Carbon/Carbon.h>
#include <OpenGL/gl.h>
#include <math.h>
#include "JBAT_MathTools.h"


static float x;

static void drawAnObject () {
	
    glColor3f(1.0f, 0.85f, 0.35f);
	
	//glutSolidSphere(10.0, 40, 40);

	glutWireSphere(1.0f, 40, 40);
	
}


@implementation BaseGLView


@synthesize mouseSensitivity, mouseInteractionEnabled, keyIntereactionEnabled;
@synthesize upIsPressed, downIsPressed, leftIsPressed, rightIsPressed, forwardIsPressed, backwardIsPressed;
@synthesize delegate;


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


#pragma mark -
#pragma mark Graphics Methods

-(void) startRenderingBase {
	
	x = 0.0;
	
//	thisContext = [self openGLContext];
//	
//	[thisContext setView:self];
//	[thisContext update];
//	[thisContext makeCurrentContext];
//	[self setOpenGLContext:thisContext];
	
	//[self becomeFirstResponder];
	
	NSOpenGLContext* ctx = [NSOpenGLContext currentContext];
	
	NSLog(@"in startRenderingBase");
	NSLog(@"Current context is %@", ctx);
	NSLog(@"This context is %@", [self openGLContext]);
	
	[self prepareOpenGL];
	[self reshape];
	
	pTimer = [NSTimer timerWithTimeInterval:(0.1) target:self selector:@selector(idle:) userInfo:nil repeats:YES];
	[[NSRunLoop currentRunLoop]addTimer:pTimer forMode:NSDefaultRunLoopMode];
	
	
	
}

-(void) stopRenderingBase {
	
	//[self resignFirstResponder];
	
	[pTimer invalidate];
	
}


- (void) prepareOpenGL { 
	[super prepareOpenGL];
	
	/*----------------------------------------------------------------*/
	//SetupRC(); (do actual setup)
	
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CCW);		// Counter clock-wise polygons face out
	glEnable(GL_CULL_FACE);		// Do not calculate inside of jet
	
	// Black background
	glClearColor(0.0f, 0.2f, 0.2f, 1.0f );	
	
	
	/*----------------------------------------------------------------*/
	


}

- (void) clearGLContext { 
	//ShutdownRC(); 
}

-(void) resize {
	
	NSLog(@"\n\nIN RESIZE\n\n");
	
}

- (void)reshape { 
	
	NSLog(@"\n\nIN RESHAPE\n\n");
	
	NSRect rect = [self bounds]; 
	//ChangeSize(rect.size.width, rect.size.height); 
	
	int w = rect.size.width;
	int h = rect.size.height;
	
	GLfloat fAspect;
	
    // Prevent a divide by zero
    if(h == 0)
        h = 1;
	
	NSLog(@"h is %d, w is %d", h, w);
	
    // Set Viewport to window dimensions
    glViewport(0, 0, w, h);
	
	
    // Reset coordinate system
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    fAspect = (float)w/(float)h;
    gluPerspective(45.0, fAspect, 0.1, 500.0);
	
	
	
	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
	
	glutPostRedisplay();
	
}



-(void) drawRect: (NSRect) bounds {
	
	
	glClearColor(0.0f, 0.2f, 0.2f, 1.0f );	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glPushMatrix();
	glLoadIdentity();
	glTranslatef(sin(x), sin(x), -10.0);	
    drawAnObject();
	glPopMatrix();
	
	
	glutSwapBuffers();	
    glFlush();

	
}

- (void)idle:(NSTimer *)pTimer {
	
	x = fullRangedAngle(x+0.1);
	
	[self drawRect:[self bounds]]; 
	
	[self setNeedsDisplay:YES];
	
	if (forwardIsPressed) {
		NSLog(@"Forward is pressed");
	}
	
	if (backwardIsPressed) {
		NSLog(@"Backward is pressed");
	}
	
	if (upIsPressed) {
		NSLog(@"Up is pressed");
	}
	
	if (downIsPressed) {
		NSLog(@"Down is pressed");
	}
	
	if (leftIsPressed) {
		NSLog(@"Left is pressed");
	}
	
	if (rightIsPressed) {
		NSLog(@"Right is pressed");
	}
}


#pragma mark -
#pragma mark Camera Interaction Methods

//- (BOOL)acceptsFirstResponder {
//	return YES;
//}
//
//- (BOOL)becomeFirstResponder {
//	return  YES;
//}
//
//- (BOOL)resignFirstResponder {
//	return YES;
//}

- (BOOL)acceptsFirstMouse:(NSEvent *)theEvent {
	return YES;
}

- (void)mouseDragged:(NSEvent *)theEvent {
	
	NSLog(@"In mouse dragged");
	
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
	
	NSLog(@"In key down, event is %@", theEvent);
	

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
	
	NSLog(@"In key up, event is %@", theEvent);
	
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
	[delegate camera]->addTheta(angle);
}

-(void) rotateCameraPhiBy:(float)angle {
	[delegate camera]->addPhi(angle);
}



@end
