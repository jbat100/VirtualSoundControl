//
//  SynthStationWindowController.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 24/10/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class VSCSoundSourceGeneratorDebugView;

@interface VSCSynthStationWindowController : NSWindowController {
    
    NSButton* testButton;
    NSView* mainParentView;
    VSCSoundSourceGeneratorDebugView *generatorDebugView;

}

@property (nonatomic, retain) IBOutlet NSButton* testButton;
@property (nonatomic, retain) IBOutlet NSView* mainParentView;
@property (nonatomic, retain) VSCSoundSourceGeneratorDebugView *generatorDebugView;

-(IBAction) testButtonClicked:(id)sender;

@end
