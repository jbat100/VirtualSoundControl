//
//  VSCSynthSourceGeneratorControlView.h
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
	
	VSCParameterControlView* parameterControlView;
	
}

@property (nonatomic, retain) VSCParameterControlView* parameterControlView;

/*
 *	Give a chance to the subclasses to react to changes to the element they 
 *	represent (after having been inform of it by any objc based messaging mechanism)
 */
-(void) updateParameterKeyIndexMap;

/*
 *	Parameter label/range/value reloading
 */
-(void) reloadParameterValues;
-(void) reloadValueForParameterWithKey:(VSCSParameter::Key)key;
-(double) doubleValueForParameterWithKey:(VSCSParameter::Key)key;

@end
