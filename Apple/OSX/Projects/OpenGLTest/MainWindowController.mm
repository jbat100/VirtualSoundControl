//
//  MainWindowController.m
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 28/03/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import "MainWindowController.h"


@implementation MainWindowController

-(void) windowDidLoad {

	
}

-(IBAction) openMeshWindowClicked:(id)sender {
	
	if (!meshWindowController) {
		meshWindowController = [[MeshWindowController alloc] init];
	}
	
	[meshWindowController showWindow:self];
	
}

@end
