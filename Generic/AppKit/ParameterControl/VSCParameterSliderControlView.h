//
//  VSCParameterControlView.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCParameterControlView.h"
#import "VSCSound.h"
#import "VSCSoundParameters.h"

#import <set>
#import <boost/bimap.hpp>

@class VSCSingleParameterSliderControlView;

@interface VSCParameterSliderControlView : VSCParameterControlView  {
	
	VSCSParameter::KeyIndexBimap parameterKeyIndexBimap;
	
	NSString* singleParameterSliderControlViewNibName;
	
	CGFloat horizontalMargin;
	CGFloat verticalMargin;

}

@property (nonatomic, copy) NSString* singleParameterSliderControlViewNibName;

@property (nonatomic, assign) CGFloat horizontalMargin;
@property (nonatomic, assign) CGFloat verticalMargin;

-(void) setparameterKeyIndexBimap:(VSCSParameter::KeyIndexBimap)keyIndexBymap;
-(const VSCSParameter::KeyIndexBimap&) parameterKeyIndexBimap;

-(VSCSParameter::Key) keyForParameterAtIndex:(NSInteger)index;
-(NSInteger) indexForParameterWithKey:(VSCSParameter::Key)key;

@end
