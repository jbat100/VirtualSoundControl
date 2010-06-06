//
//  CameraControlWindowController.h
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 11/04/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "JBAT_Camera.h"


@interface CameraControlWindowController : NSWindowController {
	
	NSPopUpButton *cameraTypeButton;
	JBAT_Camera *camera;

}

@end
