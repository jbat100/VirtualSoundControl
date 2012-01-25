//
//  VSCSingleParameterControlViewProtocol.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 23/12/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@protocol VSCSingleParameterControlViewProtocol

@property (nonatomic, copy) NSString* label;
@property (nonatomic, assign) VSCSParameter::ValueRange valueRange;
@property (nonatomic, assign) VSCSParameter::Key key; 
@property (nonatomic, assign) double doubleValue; 

@property (assign) id<VSCParameterListenerProtocol> listener;

-(void) updateInterface;


@end
