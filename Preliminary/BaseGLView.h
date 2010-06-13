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


@protocol GLViewInteractionDelegate

-(JBAT_LightSet*) lightSet; // to be able to call the opengl draw operations 
-(JBAT_Camera*) camera;

@end


@interface BaseGLView : NSOpenGLView {
	
	// used to update screen
	NSTimer *pTimer;

	float mouseSensitivity;
	
	BOOL mouseInteractionEnabled;
	BOOL keyIntereactionEnabled;
	
	
	// KEY TRACKING
	
	BOOL upIsPressed;
	BOOL downIsPressed;
	
	BOOL leftIsPressed;
	BOOL rightIsPressed;
	
	BOOL forwardIsPressed;
	BOOL backwardIsPressed;
	
	NSOpenGLContext *thisContext;
	
	id <GLViewInteractionDelegate> delegate;
	
}


@property (nonatomic) float mouseSensitivity;

@property (nonatomic) BOOL mouseInteractionEnabled;
@property (nonatomic) BOOL keyIntereactionEnabled;

@property (nonatomic) BOOL upIsPressed;
@property (nonatomic) BOOL downIsPressed;
@property (nonatomic) BOOL leftIsPressed;
@property (nonatomic) BOOL rightIsPressed;
@property (nonatomic) BOOL forwardIsPressed;
@property (nonatomic) BOOL backwardIsPressed;

@property (assign) id <GLViewInteractionDelegate> delegate;

-(void) startRenderingBase;
-(void) stopRenderingBase;
- (void) prepareOpenGL;

-(void) rotateCameraThetaBy:(float)angle;
-(void) rotateCameraPhiBy:(float)angle;

- (void)reshape;


@end
