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
    generatorDebugView.frame = mainParentView.bounds;
    
    NSLog(@"generatorDebugView: %@, frame: %@ bounds: %@", generatorDebugView, NSStringFromRect(generatorDebugView.frame), NSStringFromRect(generatorDebugView.bounds));
    NSLog(@"mainParentView: %@, frame: %@ bounds: %@", mainParentView, NSStringFromRect(mainParentView.frame), NSStringFromRect(mainParentView.bounds));
    
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
