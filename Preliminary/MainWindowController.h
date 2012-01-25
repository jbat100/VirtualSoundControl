//
//  MainWindowController.h
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 28/03/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MeshWindowController.h"

@interface MainWindowController : NSWindowController {
	
	MeshWindowController *meshWindowController;

}

-(IBAction) openMeshWindowClicked:(id)sender;

@end
