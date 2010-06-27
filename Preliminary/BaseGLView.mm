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

	glutWireSphere(1.0f, 15, 15);
	
}


@implementation BaseGLView


//@synthesize mouseSensitivity, interactionEnabled, rendering;
//@synthesize upIsPressed, downIsPressed, leftIsPressed, rightIsPressed, forwardIsPressed, backwardIsPressed;
@synthesize baseEnvironment;


- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
		

		
    }
    return self;
}

-(void) setup {
	
}

#pragma mark -
#pragma mark Graphics Methods


- (void) prepareOpenGL { 
	[super prepareOpenGL];
	
	x = 0.0;
	
	/*----------------------------------------------------------------*/
	//SetupRC(); (do actual setup)
	
	glEnable(GL_DEPTH_TEST);	// Hidden surface removal
	glFrontFace(GL_CW);		// Counter clock-wise polygons face out
	glEnable(GL_CULL_FACE);		// Do not calculate inside of jet
	
	// Black background
	glClearColor(0.0f, 0.2f, 0.2f, 1.0f );	
	
	[baseEnvironment camera]->updateFPSFrame();
	
	
	/*----------------------------------------------------------------*/
	
	[self setMeshDisplayMode:[baseEnvironment meshDisplayMode]];

}

- (void) clearGLContext { 
	//ShutdownRC(); 
}


- (void)reshape { 
	
	NSRect rect = [self bounds]; 
	//ChangeSize(rect.size.width, rect.size.height); 
	
	int w = rect.size.width;
	int h = rect.size.height;
	
	GLfloat fAspect;
	
    // Prevent a divide by zero
    if(h == 0)
        h = 1;
	
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
	
	glLoadIdentity();
	[baseEnvironment camera]->applyGLCameraTransform();
	

	
	glPushMatrix();
	//glTranslatef(sin(x), sin(x), 0.0);	
	drawAnObject();
	glPopMatrix();
	
	glutSwapBuffers();	
    glFlush();
	
}

- (void)idle:(NSTimer *)pTimer {
	
	x = fullRangedAngle(x+0.1);
	
	[self drawRect:[self bounds]]; 
	
	[self setNeedsDisplay:YES];
	
}

-(void) setMeshDisplayMode:(MeshDisplayMode)meshDisplayMode {
	if (meshDisplayMode == MeshDisplayModeSurface) {
		glPolygonMode (GL_FRONT_AND_BACK, GL_FILL);
	}
	else if (meshDisplayMode == MeshDisplayModeWireframe) {
		glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
	}
	else if (meshDisplayMode == MeshDisplayModePoints) {
		glPolygonMode (GL_FRONT_AND_BACK, GL_POINT);
	}

}

#pragma mark -
#pragma mark Camera Interaction Methods



- (BOOL)acceptsFirstMouse:(NSEvent *)theEvent {
	return YES;
}

- (void)mouseDragged:(NSEvent *)theEvent {
	
	//NSLog(@"In mouse dragged");
	//NSLog(@"Delta X is %f, Y is %f", [theEvent deltaX], [theEvent deltaY]);
	
	float sensitivity = [baseEnvironment camera]->getRotationSpeed();
	[baseEnvironment camera]->addTheta(sensitivity*[theEvent deltaX]);
	[baseEnvironment camera]->addPhi(-sensitivity*[theEvent deltaY]);
	[baseEnvironment cameraFrameChanged];
	
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





@end
