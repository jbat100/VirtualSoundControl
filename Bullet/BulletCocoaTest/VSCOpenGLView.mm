//
//  VSCOpenGLView.m
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 21/04/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "VSCOpenGLView.h"

#import "VSCBaseApplication.h"
#import "VSCControlSetup.h"
#import "VSCKeyboard.h"

#include <OpenGL/gl.h>
#include <iostream>


@implementation VSCOpenGLView

@synthesize lastDrawDate, delegate;


-(id) initWithFrame:(NSRect)frameRect pixelFormat:(NSOpenGLPixelFormat *)format {
	
	if ((self = [super initWithFrame:frameRect pixelFormat:format])) {
		[self startTimer];
	}
	
	return self;
	
}

-(id) initWithCoder:(NSCoder *)aDecoder {
	
	if ((self = [super initWithCoder:aDecoder])) {
		[self startTimer];
	}
	
	return self;
	
}

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
		[self startTimer];
		
    }
    return self;
}

-(void) startTimer {
	
	drawTimer = [NSTimer scheduledTimerWithTimeInterval:0.05 
												 target:self 
											   selector:@selector(drawTimerCallback) 
											   userInfo:nil 
												repeats:YES];
	
	[drawTimer fire];
	
}

-(void) dealloc {
	[drawTimer invalidate];
	[super dealloc];
}

-(void) drawTimerCallback {
	if ([delegate baseApplication]) {
		[delegate baseApplication]->simulate();
		[delegate baseApplication]->stepCamera();
		std::cout << [delegate baseApplication]->cameraStateString() << std::endl;
	}
	[self setNeedsDisplay:YES];
}

#pragma mark - Input functions

/* // DEMOAPP Callbacks
virtual void keyboardCallback(unsigned char key, int x, int y);
virtual void keyboardUpCallback(unsigned char key, int x, int y) {}
virtual void specialKeyboard(int key, int x, int y){}
virtual void specialKeyboardUp(int key, int x, int y){}
virtual void reshape(int w, int h);
virtual void mouseFunc(int button, int state, int x, int y);
virtual void mouseMotionFunc(int x,int y);
 */

-(BOOL) acceptsFirstMouse:(NSEvent *)theEvent {
	return YES;
}

-(BOOL) acceptsFirstResponder {
	return YES;
}

-(void)mouseDown:(NSEvent*)theEvent {
	
	if ([theEvent modifierFlags] & NSAlternateKeyMask) {

	}
	else {

	}
	
	NSPoint p1 = [theEvent locationInWindow];
	NSPoint p2 = [self convertPoint:p1 fromView:(NSView*)[[self window] contentView]];

    CGSize s = self.frame.size;
	
	/*
	if ([theEvent buttonNumber] == NSRightMouseDown) {
        if ([delegate baseApplication]) 
            [delegate baseApplication]->mouseDown(VSCMouseButtonRight, p2.x, s.height-p2.y);
	}
    else if ([theEvent buttonNumber] == NSLeftMouseDown) {
        if ([delegate baseApplication]) 
            [delegate baseApplication]->mouseDown(VSCMouseButtonLeft, p2.x, s.height-p2.y);
	}
	 */
	
}

-(void)mouseDragged:(NSEvent *)theEvent {
	NSLog(@"In mouse dragged, event is %@", theEvent);
	
	// not expcting deltas seems like so really screws things up...
	int deltaX = [theEvent deltaX];
	int deltaY = [theEvent deltaY];
	
	NSPoint p1 = [theEvent locationInWindow];
	NSPoint p2 = [self convertPoint:p1 fromView:(NSView*)[[self window] contentView]];
	
	CGSize s = self.frame.size;
	
	if ([delegate baseApplication])
		[delegate baseApplication]->mouseMotion(deltaX, -deltaY, p2.x, s.height-p2.y);
	
}

-(void)mouseUp:(NSEvent *)theEvent {
	
	NSPoint p1 = [theEvent locationInWindow];
	NSPoint p2 = [self convertPoint:p1 fromView:(NSView*)[[self window] contentView]];
	
	CGSize s = self.frame.size;
	
	/*
	if ([theEvent buttonNumber] == NSRightMouseDown) {
        if ([delegate baseApplication]) 
            [delegate baseApplication]->mouseUp(VSCMouseButtonRight, p2.x, s.height-p2.y);
	}
    else if ([theEvent buttonNumber] == NSLeftMouseDown) {
        if ([delegate baseApplication]) 
            [delegate baseApplication]->mouseUp(VSCMouseButtonLeft, p2.x, s.height-p2.y);
	}
	 */
	
}

-(void)keyDown:(NSEvent *)theEvent {
	
	
	
	//NSLog(@"In key down, event is %@", theEvent);
	//wchar_t wc = *[[theEvent charactersIgnoringModifiers] cStringUsingEncoding:NSUTF16StringEncoding];
	
    if ([[theEvent characters] length] > 0) {
        
        wchar_t wc = [[theEvent characters] characterAtIndex:0];
        
        //unichar uc = [[theEvent characters] characterAtIndex:0];
        
        //std::wcout << "\nDEBUG: Size of wchar_t is " << sizeof(wc) << ", size of VSCUpArrowFunctionKey is " << sizeof(VSCUpArrowFunctionKey); 
        //std::wcout << "\nwc is: " << wc;
        //std::cout << "\nhex wc is : " << wideCharString(wc) << std::endl;
        //std::cout << "\nuc is: " << uc;
        
        VSCKeyboardCombination comb((wchar_t)wc, [theEvent modifierFlags]);
        
        //std::wcout << "----> Created " << comb << std::endl;
        
        if ([delegate baseApplication]) 
            [delegate baseApplication]->keyDown(comb);
        
    }
    

}

-(void)keyUp:(NSEvent *)theEvent {
	//NSLog(@"In key up, event is %@", theEvent);
    
    if ([[theEvent characters] length] > 0) {
        
        wchar_t wc = [[theEvent characters] characterAtIndex:0];
        
        //unichar uc = [[theEvent characters] characterAtIndex:0];
        
        //std::wcout << "\nDEBUG: Size of wchar_t is " << sizeof(wc) << ", size of VSCUpArrowFunctionKey is " << sizeof(VSCUpArrowFunctionKey); 
        //std::wcout << "\nwc is: " << wc;
        //std::cout << "\nhex wc is : " << wideCharString(wc) << std::endl;
        //std::cout << "\nuc is: " << uc;
        
        VSCKeyboardCombination comb((wchar_t)wc, [theEvent modifierFlags]);
        
        //std::wcout << "----> Created " << comb << std::endl;
        
        if ([delegate baseApplication]) 
            [delegate baseApplication]->keyUp(comb);
        
    }
    
}

#pragma mark - GL functions

- (void)drawRect:(NSRect)dirtyRect {
	
    // Drawing code here.
	
	//NSLog(@"in drawRect");
	
	if ([delegate baseApplication]) {
		//NSLog(@"baseApplication exists");
        [delegate baseApplication]->display();
	}
	
	glutSwapBuffers();	
    glFlush();
	
	/*
	if (lastDrawDate) {
		NSTimeInterval interval = -[lastDrawDate timeIntervalSinceDate:[NSDate date]];
		NSLog(@"Draw time interval is %f", interval);
	}
	self.lastDrawDate = [NSDate date];
	 */
	
}


-(void) setup {
	
}

-(void) prepareOpenGL {
	
	GLfloat light_ambient[] = { btScalar(0.2), btScalar(0.2), btScalar(0.2), btScalar(1.0) };
	GLfloat light_diffuse[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0) };
	GLfloat light_specular[] = { btScalar(1.0), btScalar(1.0), btScalar(1.0), btScalar(1.0 )};
	
	/*	light_position is NOT default value	*/
	GLfloat light_position0[] = { btScalar(1.0), btScalar(10.0), btScalar(1.0), btScalar(0.0 )};
	GLfloat light_position1[] = { btScalar(-1.0), btScalar(-10.0), btScalar(-1.0), btScalar(0.0) };
	
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position0);
	
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	
	
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);
	
	glClearColor(btScalar(0.7),btScalar(0.7),btScalar(0.7),btScalar(0));
	
}

-(void) reshape {
	
	NSLog(@"In reshape");
	
	NSRect rect = [self bounds]; 
	//ChangeSize(rect.size.width, rect.size.height); 
	
	int w = rect.size.width;
	int h = rect.size.height;
	
	if ([delegate baseApplication])
		[delegate baseApplication]->reshape(w, h);
	
	glutPostRedisplay();
	
}


@end
