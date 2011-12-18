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

-(void) addParameterKey:(VSCSParameter::Key)k;
-(void) removeParameterKey:(VSCSParameter::Key)k;
-(void) addParameterKeys:(VSCSParameter::KeyList)keyList;
-(void) removeParameterKeys:(VSCSParameter::KeyList)keyList;

-(void) setLabel:(NSString*)label forParameterKey:(VSCSParameter::Key)k;
-(void) setDefaultLabelForParameterKey:(VSCSParameter::Key)k;
-(void) setDefaultLabelForAllParameterKeys;

-(void) setRange:(VSCSParameter::ValueRange)valueRange forParameterKey:(VSCSParameter::Key)k;
-(void) setDefaultRangeForParameterKey:(VSCSParameter::Key)k;
-(void) setDefaultLabelForAllParameterKeys;

-(NSString*) getLabelForParameterKey:(VSCSParameter::Key)k;
-(VSCSParameter::ValueRange) getRangeForParameterKey:(VSCSParameter::Key)k;

/*
 *	Which parameters is the view meant to control, at what index
 */
-(const VSCSParameter::KeyList&) keyList;
-(const VSCSParameter::KeyLabelMap&) keyLabelMap;
-(const VSCSParameter::KeyRangeMap&) keyRangeMap;

@end


