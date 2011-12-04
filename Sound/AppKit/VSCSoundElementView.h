//
//  VSCSoundGeneratorControlView.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 07/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCParameterControlView.h"
#import "VSCSoundElement.h"

#import <map>

/*
 * Change to control view to have a controllable element
 */

@interface VSCSoundElementView : NSView <VSCParameterControlViewDelegate> {
	
	VSCSoundElementPtr soundElement;
	
	NSMutableArray* parameterControlViews;
	
}

@property (nonatomic, retain) NSMutableArray* parameterControlViews;

-(void) setSoundElement:(VSCSoundElementPtr)element;
-(VSCSoundElementPtr) getSoundElement;

/*
 *	Parameter label/range/value reloading
 */
-(void) reloadParameterValues;
-(void) reloadValueForParameterWithKey:(VSCSParameter::Key)key;
-(double) doubleValueForParameterWithKey:(VSCSParameter::Key)key;

@end
