//
//  SynthStationWindowController.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 24/10/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSynthStationWindowController.h"


@implementation VSCSynthStationWindowController

@synthesize testButton;

-(void) windowDidLoad {
	NSLog(@"%@ windowDidLoad", self);
}

-(void) windowWillLoad {
	NSLog(@"%@ windowWillLoad", self);
}

-(IBAction) testButtonClicked:(id)sender {
	NSLog(@"Test button clicked");
}

@end
