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

typedef enum _VSCParameterControlOptions {
	VSCParameterControlOptionNone = 0,
	VSCParameterControlOptionDisplayLabel = 1 << 0,
	VSCParameterControlOptionDisplayRange = 1 << 1,
	VSCParameterControlOptionDisplayNumeric = 1 << 2,
} VSCParameterControlOptions;


/*
 *  VSCParameterControlViewProtocol
 *
 *	This protocol defines an interface for a control view 
 *
 */

@protocol VSCParameterControlViewProtocol <NSObject>

@required

/*
 *	Interface creation
 */
-(void) createInterfaceForParameterCount:(NSUInteger)count;
-(void) createInterfaceForParameterCount:(NSUInteger)count 
							 withOptions:(VSCParameterControlOptions)options;

/*
 *	Parameter control limits
 */
-(void) setLowerLimit:(VSCSFloat)low 
		  higherLimit:(VSCSFloat)high 
  forParameterAtIndex:(NSUInteger)index;

/*
 *	Value/Label changes
 */
-(void) setVSCSFloat:(VSCSFloat)f forParameterAtIndex:(NSUInteger)index;
-(void) setLabel:(NSString*)l forParameterAtIndex:(NSUInteger)index;
-(VSCSFloat) getParameterAtIndex:(NSUInteger)index;

@end


@protocol VSCParameterControlViewDelegate

@optional

-(void) parameterControlView:(id<VSCParameterControlViewProtocol>)view 
	 changedParameterAtIndex:(NSUInteger)index;

@end


