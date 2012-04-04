//
//  BaseDisplayView.h
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 28/03/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "JBAT_LightSet.h"
#import "JBAT_Camera.h"
#import "LightsControlViewController.h"


@interface ViewComponentsControlViewController : NSViewController {
	
	IBOutlet NSTabView *controlTabView;
	LightsControlViewController *lightsControlViewController;	
	
}

@property (assign) LightsControlViewController *lightsControlViewController;	

- (void) setup;
- (void) updateInterface;


@end
