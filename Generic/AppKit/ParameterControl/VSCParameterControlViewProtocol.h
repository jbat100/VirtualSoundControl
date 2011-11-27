//
//  VSCSoundParameterViewProtocol.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 13/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCSound.h"
#import "VSCSoundParameters.h"

#import <set>
#import <map>
#import <boost/bimap.hpp>


/*
 *  VSCParameterControlViewProtocol
 *
 *	This protocol defines an interface for a control view 
 *
 */

@protocol VSCParameterControlViewProtocol <NSObject>

@required

/*
 *	Interface creation (call after changing parameterKeys)
 */
-(void) createInterface;

/*
 *	Value/Label changes
 */
-(void) setDoubleValue:(double)f forParameterWithKey:(VSCSParameter::Key)key;
-(double) getDoubleValueForParameterWithKey:(VSCSParameter::Key)key;

/*
 *	Which parameters is the view meant to control, at what index
 */
-(VSCSParameterKeySet&) parameterKeys;
-(VSCSParameterLabelMap&) parameterLabels;
-(VSCSParameterRangeMap&) parameterRanges;

@end


@protocol VSCParameterControlViewDelegate

@optional

-(void) parameterControlView:(id<VSCParameterControlViewProtocol>)view 
	 changedParameterWithKey:(VSCSParameter::Key)key;

@end


