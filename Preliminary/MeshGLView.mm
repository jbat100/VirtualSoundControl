//
//  MeshGLView.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 20/06/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "MeshGLView.h"
#import "JBAT_BufferedMesh.h"
#import <iostream>


@implementation MeshGLView

@synthesize meshSource;

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    return self;
}

- (void) prepareOpenGL { 
	[super prepareOpenGL];	
}


-(void) drawRect: (NSRect) bounds {
	
	glClearColor(0.0f, 0.2f, 0.2f, 1.0f );	
	glClear(GL_COLOR_BUFFER_BIT);
	
	glLoadIdentity();
	[delegate camera]->applyGLCameraTransform();
	
	
	glPushMatrix();
	
	//NSLog(@"Drawing mesh %@", [meshSource mesh]);
	if ([meshSource mesh]) {
		//std::cout << "\nDrawing " << *[meshSource mesh];
		[meshSource mesh]->draw();
	}
	
	glPopMatrix();
	
	glutSwapBuffers();	
    glFlush();
	
}

@end
