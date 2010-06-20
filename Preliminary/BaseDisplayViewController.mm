//
//  BaseDisplayViewController.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 13/06/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "BaseDisplayViewController.h"
#include <Carbon/Carbon.h>
#import <iostream>



@implementation BaseDisplayViewController

@synthesize camera, lightSet, glView;

-(void) setup {
	
	interactionEnabled = NO;
	rendering = NO;
	upIsPressed = NO;
	downIsPressed = NO;
	leftIsPressed = NO;
	rightIsPressed = NO;
	forwardIsPressed = NO;
	backwardIsPressed = NO;
	
	camera = new JBAT_Camera();
	lightSet = new JBAT_LightSet();
	
	camera->SetOrigin(-20.0, 0.0, 0.0);
	
	[glView setup];
	[glView setDelegate:self];
	
	[renderingButton setState:rendering ? 1 : 0];
	[interactionButton setState:interactionEnabled ? 1 : 0];
	

}


-(IBAction)	lightSetClicked:(id)sender {
	
	if (!lightSetWindowController) {
		lightSetWindowController = [[LightSetWindowController alloc] init];
		[lightSetWindowController setDataSource:self];
	}
	
	[lightSetWindowController showWindow:self];
	
}

-(IBAction) cameraClicked:(id)sender {
	
	if (!cameraControlWindowController) {
		cameraControlWindowController = [[CameraControlWindowController alloc] init];
		[cameraControlWindowController setDataSource:self];
	}
	
	[cameraControlWindowController showWindow:self];
	[cameraControlWindowController updateTextFields];
	[cameraControlWindowController updateInteractionSliders];
	
}

-(IBAction) renderingClicked:(id)sender {
	
	if (![renderingButton state]) {
		NSLog(@"Stopped rendering");
		rendering = NO;
		[self stopRendering];
	}
	else {
		NSLog(@"Started rendering");
		rendering = NO;
		[self startRendering];
	}
	
}



-(IBAction) interactionClicked:(id)sender {
	NSLog(@"Interaction clicked");
	if ([interactionButton state]) 
		interactionEnabled = YES;
	else 
		interactionEnabled = NO;
}

- (void)keyDown:(NSEvent *)theEvent {
	
	//NSLog(@"In BaseDisplayViewController key down, event is %@", theEvent);
	if (!interactionEnabled) {
		NSLog(@"Interaction is disabled");
	}
	
	else {
		
		if ([theEvent modifierFlags]) {
			
			NSLog(@"There is at least one modeifier flag");
			
		}
		
		unsigned short code = [theEvent keyCode];
		
		if (code == kVK_ANSI_A || code == kVK_LeftArrow) {
			//NSLog(@"A Pressed");
			leftIsPressed = YES;
			return;
		}
		
		if (code == kVK_ANSI_W || code == kVK_UpArrow) {
			//NSLog(@"W Pressed");
			forwardIsPressed = YES;
			return;
		}
		
		if (code == kVK_ANSI_D || code == kVK_RightArrow) {
			//NSLog(@"D Pressed");
			rightIsPressed = YES;
			return;
		}
		
		if (code == kVK_ANSI_S || code == kVK_DownArrow) {
			//NSLog(@"S Pressed");
			backwardIsPressed = YES;
			return;
		}
	
	}
}

- (void)keyUp:(NSEvent *)theEvent {
	
	//NSLog(@"In BaseDisplayViewController key up, event is %@", theEvent);
	if (!interactionEnabled) {
		//NSLog(@"Interaction is disabled");
	}
	
	else {
		
		unsigned short code = [theEvent keyCode];
		
		
		if (code == kVK_ANSI_A || code == kVK_LeftArrow) {
			//NSLog(@"A Released");
			leftIsPressed = NO;
			return;
		}
		
		if (code == kVK_ANSI_W || code == kVK_UpArrow) {
			//NSLog(@"W Released");
			forwardIsPressed = NO;
			return;
		}
		
		if (code == kVK_ANSI_D || code == kVK_RightArrow) {
			//NSLog(@"D Released");
			rightIsPressed = NO;
			return;
		}
		
		if (code == kVK_ANSI_S || code == kVK_DownArrow) {
			//NSLog(@"S Released");
			backwardIsPressed = NO;
			return;
		}
	}
	
}

-(void) cameraFrameChanged {
	camera->updateFPSFrame();
	//std::cout << "hello! " << *camera;
	[cameraControlWindowController updateTextFields];
}


-(void) startRendering {
	
	[glView prepareOpenGL];
	[glView reshape];
	
	renderingTimer = [NSTimer timerWithTimeInterval:(0.04) target:glView selector:@selector(idle:) userInfo:nil repeats:YES];
	[[NSRunLoop currentRunLoop]addTimer:renderingTimer forMode:NSDefaultRunLoopMode];
	
	interactionTimer = [NSTimer timerWithTimeInterval:(0.04) target:self selector:@selector(updateCameraPosition) userInfo:nil repeats:YES];
	[[NSRunLoop currentRunLoop]addTimer:interactionTimer forMode:NSDefaultRunLoopMode];
	
}

-(void) stopRendering {

	[renderingTimer invalidate];
	[interactionTimer invalidate];
	
}

-(void) updateCameraPosition {
	
	if (interactionEnabled) {
		
		if (forwardIsPressed) {
			//NSLog(@"Move forward by %f", keySensitivity);
			camera->moveForward();
			[cameraControlWindowController updateTextFields];
		}
		
		if (backwardIsPressed) {
			//NSLog(@"Move bacward by %f", keySensitivity);
			camera->moveBackward();
			[cameraControlWindowController updateTextFields];
		}
		
		if (upIsPressed) {
			//NSLog(@"Up is pressed");
			camera->moveUp();
			[cameraControlWindowController updateTextFields];
		}
		
		if (downIsPressed) {
			//NSLog(@"Down is pressed");
			camera->moveDown();
			[cameraControlWindowController updateTextFields];
		}
		
		if (leftIsPressed) {
			//NSLog(@"Move left by %f", keySensitivity);
			camera->moveRight();
			[cameraControlWindowController updateTextFields];
		}
		
		if (rightIsPressed) {
			//NSLog(@"Move right by %f", keySensitivity);
			camera->moveLeft();
			[cameraControlWindowController updateTextFields];
		}
		
	}
	
}


@end
