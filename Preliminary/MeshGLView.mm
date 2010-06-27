//
//  MeshGLView.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 20/06/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "MeshGLView.h"

#import "JBAT_BufferedMesh.h"
#import "JBAT_Light.h"
#include "JBAT_MathTools.h"

#import <iostream>

static float x;


@implementation MeshGLView

@synthesize meshEnvironment;

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    return self;
}

- (void) prepareOpenGL { 
	[super prepareOpenGL];	
	
	x = 0.0;
	
	JBAT_SetupOpenGLLighting(JBAT_OpenGLLightingSetupDefault);
	JBAT_SetUpOpenGLMeshSetup(JBAT_OpenGLMeshTypeDefault, JBAT_OpenGLColorSetupDefault);
	
}


-(void) drawRect: (NSRect) bounds {
	
	x = fullRangedAngle(x+0.1);
	
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f );	
	glClear(GL_COLOR_BUFFER_BIT);
	
	[baseEnvironment lightSet]->applyGL();
	
	glLoadIdentity();
	[baseEnvironment camera]->applyGLCameraTransform();
	
	if ([baseEnvironment floor]) {
		glPushMatrix();
			glRotatef(3.1415, 0.0, 0.0, 0.1);
			glPolygonMode (GL_FRONT_AND_BACK, GL_LINE);
			[baseEnvironment floor]->draw();
			[self setMeshDisplayMode:[baseEnvironment meshDisplayMode]];
		glPopMatrix();
	}
	
	glPushMatrix();
		//NSLog(@"Drawing mesh %@", [meshEnvironment mesh]);
		if ([meshEnvironment mesh]) {
			glTranslatef(0.0, 0.0, 0.0);	
			//std::cout << "\nDrawing " << *[meshEnvironment mesh];
			[meshEnvironment mesh]->draw();
		}
	glPopMatrix();
	
	glutSwapBuffers();	
    glFlush();
	
}

@end
