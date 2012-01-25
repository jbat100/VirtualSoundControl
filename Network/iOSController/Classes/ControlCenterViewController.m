//
//  ControlViewController.m
//  iOSController
//
//  Created by Jonathan Thorpe on 13/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "ControlCenterViewController.h"
#import "ControlCenterSetup.h"
#import "ControlStation.h"
#import "ControlViewSetup.h"

@implementation ControlCenterViewController

@synthesize controlSetup;

// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
/*
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization.
    }
    return self;
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	controlSetup = [[ControlCenterSetup alloc] init];
	[controlSetup setPreset:ControlCenterSetupPresetFullScreenTouch];
	[self applyControlSetup];
	[self registerControlViewsWithControlStation];
	
}


/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations.
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
	
	[self unregisterControlViewsWithControlStation];
	
	[controlSetup release];
	controlSetup = nil;
}


- (void)dealloc {
    [super dealloc];
}

#pragma mark - 
#pragma mark Control Setup

-(void) applyControlSetup {
	[controlSetup applySetupToView:self.view];
}

-(void) registerControlViewsWithControlStation {
	
	for (ControlViewSetup* setup in controlSetup.controlViewSetups) {
		[[ControlStation instance] addControlView:setup.view];
	}
	
}

-(void) unregisterControlViewsWithControlStation {
	
	for (ControlViewSetup* setup in controlSetup.controlViewSetups) {
		[[ControlStation instance] removeControlView:setup.view];
	}
	
}

@end
