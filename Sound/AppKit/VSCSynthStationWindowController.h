//
//  SynthStationWindowController.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 24/10/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface VSCSynthStationWindowController : NSWindowController {
	
	NSButton* testButton;

}

@property (nonatomic, retain) IBOutlet NSButton* testButton;

-(IBAction) testButtonClicked:(id)sender;

@end
