//
//  VSCSynthSourceGeneratorControlView.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 07/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCParameterControlView.h"
#import "VSCParameterControlSlidersView.h"


@interface VSCSynthSourceGeneratorControlView : NSView <VSCParameterControlViewDelegate, VSCParameterControlViewDataSource> {
	
	VSCParameterControlSlidersView* parameterControlSlidersView;

}

@property (nonatomic, retain) IBOutlet VSCParameterControlSlidersView* parameterControlSlidersView;

-(NSArray*) parameterKeys;
-(NSDictionary*) parameterIndexPaths;
-(NSDictionary*) parameterDisplayStrings;

-(void) customInit;

-(void) addParameterWithKey:(NSString*)key;
-(void) removeParameterWithKey:(NSString*)key;
-(void) removeAllParameters;

@end
