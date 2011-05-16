//
//  VSCOpenGLView.m
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 21/04/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "VSCOpenGLView.h"
#import "VSCBaseApplication.h"
#include <OpenGL/gl.h>


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
	
	drawTimer = [NSTimer scheduledTimerWithTimeInterval:0.01 
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
	
	if ([theEvent buttonNumber] == NSRightMouseDown) {
        if ([delegate basicDemo]) 
            [delegate basicDemo]->mouseDown(VSCMouseButtonRight, p2.x, s.height-p2.y);
	}
    else if ([theEvent buttonNumber] == NSLeftMouseDown) {
        if ([delegate basicDemo]) 
            [delegate basicDemo]->mouseDown(VSCMouseButtonLeft, p2.x, s.height-p2.y);
	}
	
}

-(void)mouseDragged:(NSEvent *)theEvent {
	NSLog(@"In mouse dragged, event is %@", theEvent);
	
	// not expcting deltas seems like so really screws things up...
	int deltaX = [theEvent deltaX];
	int deltaY = [theEvent deltaY];
	
	NSPoint p1 = [theEvent locationInWindow];
	NSPoint p2 = [self convertPoint:p1 fromView:(NSView*)[[self window] contentView]];
	
	CGSize s = self.frame.size;
	
	if ([delegate basicDemo])
		[delegate basicDemo]->mouseMotion(deltaX, -deltaY, p2.x, s.height-p2.y);
	
}

-(void)mouseUp:(NSEvent *)theEvent {
	
	NSPoint p1 = [theEvent locationInWindow];
	NSPoint p2 = [self convertPoint:p1 fromView:(NSView*)[[self window] contentView]];
	
	CGSize s = self.frame.size;
	
	if ([theEvent buttonNumber] == NSRightMouseDown) {
        if ([delegate basicDemo]) 
            [delegate basicDemo]->mouseUp(VSCMouseButtonRight, p2.x, s.height-p2.y);
	}
    else if ([theEvent buttonNumber] == NSLeftMouseDown) {
        if ([delegate basicDemo]) 
            [delegate basicDemo]->mouseUp(VSCMouseButtonLeft, p2.x, s.height-p2.y);
	}
	
}

-(void)keyDown:(NSEvent *)theEvent {
	// NSLog(@"In key down, event is %@, key is %c", theEvent, *[[theEvent charactersIgnoringModifiers] cStringUsingEncoding:NSUTF8StringEncoding]);

    /*
	if ([delegate basicDemo]) 
		[delegate basicDemo]->keyboardCallback((unsigned char)*[[theEvent charactersIgnoringModifiers] cStringUsingEncoding:NSUTF8StringEncoding], 0, 0);
     */
}

-(void)keyUp:(NSEvent *)theEvent {
	NSLog(@"In key up, event is %@", theEvent);
}

#pragma mark - GL functions

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
	
	//NSLog(@"in drawRect");
	
	if ([delegate basicDemo]) {
		//NSLog(@"basicDemo exists");
		[delegate basicDemo]->simulate();
        [delegate basicDemo]->display();
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
	
	if ([delegate basicDemo])
		[delegate basicDemo]->reshape(w, h);
	
	glutPostRedisplay();
	
}


@end
