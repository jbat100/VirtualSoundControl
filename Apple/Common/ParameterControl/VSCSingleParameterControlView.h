//
//  VSCSingleParameterControlView.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 10/12/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCSoundParameters.h"
#import "VSCParameterListenerProtocol.h"

@interface VSCSingleParameterControlView : NSView {
	
	NSString* label;
	VSCSParameter::ValueRange valueRange;
	VSCSParameter::Key key; 
	double doubleValue; 
	
	id<VSCParameterListenerProtocol> listener;
	
}

@property (nonatomic, copy) NSString* label;
@property (nonatomic, assign) VSCSParameter::ValueRange valueRange;
@property (nonatomic, assign) VSCSParameter::Key key; 
@property (nonatomic, assign) double doubleValue; 

@property (assign) id<VSCParameterListenerProtocol> listener;

-(void) updateInterface;

@end
