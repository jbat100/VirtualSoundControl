//
//  SoundStationDocument.h
//  VSoundStation
//
//  Created by Jonathan Thorpe on 31/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
@class SBJSON;
@class TouchDisplayView;
@class ClientController;
@class MultiDisplayView;

@interface SoundStationDocument : NSDocument {
	
	SBJSON* jsonParser;
	
	NSButton* searchButton;
	NSButton* connectButton;
	
	IBOutlet NSMatrix* accelerationSliderMatrix;
	
	IBOutlet TouchDisplayView* touchDisplayView;
	IBOutlet MultiDisplayView* multiDisplayView;
	
	IBOutlet ClientController* clientController;

}

@property (nonatomic, retain) IBOutlet NSButton* testButton;

-(IBAction) buttonTouched:(id)sender;

-(void) updateViewWithStateString:(NSString*)stateString;

@end
