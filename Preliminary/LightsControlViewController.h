//
//  LightsControlView.h
//  MeshFrames3
//
//  Created by Jonathan Thorpe on 05/04/2010.
//  Copyright 2010 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "LightControlViewController.h"
#import "JBAT_LightSet.h"
#import "JBAT_Light.h"

@protocol LightsControlViewControllerDataSource

-(JBAT_LightSet*) lightSet;

@end



@interface LightsControlViewController : NSViewController <LightControlViewControllerDataSource,
NSTableViewDataSource, NSTableViewDelegate> {
	
	IBOutlet NSView *lightControlView;
	IBOutlet NSTableView *lightsTable;
	
	IBOutlet NSButton *addLightButton;
	IBOutlet NSButton *removeLightButton;
	
	LightControlViewController *lightControlViewController;
	
	NSInteger activeLightIndex;
	
	id<LightsControlViewControllerDataSource> dataSource;

}

@property (assign, nonatomic) NSTableView *lightsTable;
@property (assign) id<LightsControlViewControllerDataSource> dataSource;

-(void) setup;
-(void) updateInterface;

-(void) addLightClicked:(id)sender;
-(void) removeLightClicked:(id)sender;
-(IBAction) printLightsButtonClicked:(id)sender;

-(NSInteger) numberOfLights;
-(JBAT_Light *) lightAtIndex:(NSInteger)index;
-(BOOL) addLight;
-(BOOL) removeLightAtIndex:(NSInteger)index;
-(void) printLightsToConsole;

@end
