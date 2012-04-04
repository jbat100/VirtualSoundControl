//
//  VSCParameterListenerProtocol.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 10/12/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCSoundParameters.h"


@protocol VSCParameterListenerProtocol

-(void) object:(id)sender changedParameterWithKey:(VSCSParameter::Key)key to:(double)val;

@end

