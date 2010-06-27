//
//  MeshGLView.h
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 24/05/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "JBAT_LightSet.h"
#import "JBAT_Camera.h"
#import "JBAT_Object.h"


typedef enum _MeshDisplayMode {
	MeshDisplayModeSurface,
	MeshDisplayModeWireframe,
	MeshDisplayModePoints
} MeshDisplayMode;


@protocol BaseGLViewEnvironment

-(JBAT_LightSet*) lightSet; // to be able to call the opengl draw operations 
-(JBAT_Camera*) camera;
-(void) cameraFrameChanged;
-(MeshDisplayMode) meshDisplayMode;
-(JBAT_Object*) floor;

@end


@interface BaseGLView : NSOpenGLView {
	

	
	NSOpenGLContext *thisContext;
	
	id <BaseGLViewEnvironment> baseEnvironment;
	
}


//@property (nonatomic) float mouseSensitivity;
//@property (nonatomic) BOOL interactionEnabled;
//@property (nonatomic) BOOL rendering;
//
//@property (nonatomic) BOOL upIsPressed;
//@property (nonatomic) BOOL downIsPressed;
//@property (nonatomic) BOOL leftIsPressed;
//@property (nonatomic) BOOL rightIsPressed;
//@property (nonatomic) BOOL forwardIsPressed;
//@property (nonatomic) BOOL backwardIsPressed;

@property (assign) id <BaseGLViewEnvironment> baseEnvironment;

-(void) setMeshDisplayMode:(MeshDisplayMode)meshDisplayMode;

-(void) setup;
-(void) prepareOpenGL;
-(void)reshape;


@end
