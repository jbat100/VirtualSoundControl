//
//  SynthStationWindowController.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 24/10/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSynthStationWindowController.h"

#import "VSCSoundSourceGeneratorDebugView.h"
#import "VSCSoundSine.h"

@implementation VSCSynthStationWindowController

@synthesize testButton;
@synthesize mainParentView;
@synthesize generatorDebugView;

-(void) windowDidLoad {
	NSLog(@"%@ windowDidLoad", self);
    
    NSNib* nib = [[NSNib alloc] initWithNibNamed:@"VSCSoundSourceGeneratorDebugView" bundle:nil];
    NSArray* topLevelObjects = nil;
    [nib instantiateNibWithOwner:self topLevelObjects:&topLevelObjects];
    NSLog(@"topLevelObjects: %@", topLevelObjects);
    self.generatorDebugView = nil;
    for (id o in topLevelObjects) {
        if ([o isKindOfClass:[VSCSoundSourceGeneratorDebugView class]]) self.generatorDebugView = o;
    }
    NSAssert(generatorDebugView, @"Bad top object class");
    generatorDebugView.bounds = mainParentView.bounds;
    [mainParentView addSubview:generatorDebugView];

}

-(void) windowWillLoad {
	NSLog(@"%@ windowWillLoad", self);
}

-(IBAction) testButtonClicked:(id)sender {
	NSLog(@"Test button clicked");
    VSCSoundSinePtr sinePtr = VSCSoundSinePtr(new VSCSoundSine());
    [[self generatorDebugView] setSoundGenerator:sinePtr];
}

@end
