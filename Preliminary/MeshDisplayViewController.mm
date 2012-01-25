//
//  MeshDisplayView.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 28/03/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "MeshDisplayViewController.h"
#import "JBAT_Camera.h"


@implementation MeshDisplayViewController

@synthesize dataSource;

-(void) awakeFromNib {
	[(MeshGLView*)glView setMeshEnvironment:self];
}

-(void) setup {
	[super setup];
}
	 

	 
-(void) startRendering {
	JBAT_SetupOpenGLClientStates(JBAT_OpenGLClientStatesDefault);
	JBAT_SetupOpenGLLighting(JBAT_OpenGLLightingSetupDefault);
	[super startRendering];
}

-(void) stopRendering {
	JBAT_SetupOpenGLClientStates(JBAT_OpenGLClientStatesOff);
	[super startRendering];
}

-(JBAT_BufferedMesh*) mesh {
	return [dataSource modelMesh];
}
								  
								  
@end
