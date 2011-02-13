//
//  TouchViewController.m
//  iOSController
//
//  Created by Jonathan Thorpe on 15/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "TouchViewController.h"
#import "TouchControlView.h"
#import "ControlStation.h"
#import "NetworkDefinitions.h"


@implementation TouchViewController

@synthesize backgroundImageView, touchRelayView;

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if (self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil]) {
        // Custom initialization
    }
    return self;
}
*/

/*
// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
}
*/


// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
	
	CGRect f = self.view.frame;
	f.origin.x = f.origin.y = 0.0;
	
	self.touchRelayView = [[[TouchControlView alloc] initWithFrame:f] autorelease];
	touchRelayView.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
	
	[self.view addSubview:touchRelayView];
	[self.view bringSubviewToFront:touchRelayView];
	
	[[ControlStation instance] addTouchRelayView:touchRelayView 
									   forKey:[NSString stringWithCString:JSON_MAIN_TOUCH_VIEW_KEY 
																 encoding:NSUTF8StringEncoding]];


}


/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
	// Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
	
	// Release any cached data, images, etc that aren't in use.
}

- (void)viewDidUnload {
	// Release any retained subviews of the main view.
	// e.g. self.myOutlet = nil;
	
	self.touchRelayView = nil;
	self.backgroundImageView = nil;
	
}


- (void)dealloc {
	
	self.touchRelayView = nil;
	self.backgroundImageView = nil;
	
    [super dealloc];
}


@end
