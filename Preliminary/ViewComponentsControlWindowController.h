//
//  ViewComponentsControlWindowController.h
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 05/04/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "ViewComponentsControlViewController.h"
#import "JBAT_LightSet.h"


@interface ViewComponentsControlWindowController : NSWindowController <LightsControlViewControllerDataSource> {
	
	IBOutlet ViewComponentsControlViewController *viewComponentsControlViewController;
	JBAT_LightSet* lightSet;

}

@property (assign) JBAT_LightSet *lightSet;

@end
