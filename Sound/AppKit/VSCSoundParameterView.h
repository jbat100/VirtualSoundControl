//
//  VSCSynthSourceGeneratorControlView.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 07/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCParameterControlView.h"
#import "VSCSoundParameters.h"

#import <map>
#import <boost/bimap.hpp>


@interface VSCSoundParameterView : NSView <VSCParameterControlViewDelegate> {
	
	VSCParameterControlView* parameterControlView;
	
	boost::bimap<VSCSParameter::Key, NSInteger> paramKeyIndexMap;

}

@property (nonatomic, retain) VSCParameterControlView* parameterControlView;

-(void) reloadParameterLabels;
-(void) reloadParameterRanges;
-(void) reloadParameterValues;
-(void) reloadValueForParameterWithKey:(VSCSParameter::Key)key;

-(VSCSParameter::Key) keyForParameterAtIndex:(NSInteger)index;
-(NSInteger) indexForParameterWithKey:(VSCSParameter::Key)key;
-(NSString*) labelForParameterWithKey:(VSCSParameter::Key)key;
-(double) doubleValueForParameterWithKey:(VSCSParameter::Key)key;
-(std::pair<double, double>) rangeForParameterWithKey:(VSCSParameter::Key)key;
-(NSInteger) numberOfParameters;

@end
