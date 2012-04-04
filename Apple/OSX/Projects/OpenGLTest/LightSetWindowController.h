//
//  LightSetWindowController.h
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 17/06/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "LightsControlViewController.h"
#import "JBAT_LightSet.h"

@protocol LightSetWindowControllerDataSource

-(JBAT_LightSet*) lightSet;

@end


@interface LightSetWindowController : NSWindowController <LightsControlViewControllerDataSource> {
	
	IBOutlet NSView *lightSetView;
	
	LightsControlViewController *lightsControlViewController;
	id<LightSetWindowControllerDataSource> dataSource;

}

@property (assign) id<LightSetWindowControllerDataSource> dataSource;

@end
