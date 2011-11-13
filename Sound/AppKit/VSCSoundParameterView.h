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
#import "VSCSoundParameterViewProtocol.h"

#import <boost/bimap.hpp>


@interface VSCSoundParameterView : NSView
<VSCParameterControlViewDelegate, VSCParameterControlViewDataSource, VSCSoundParameterViewProtocol> {
	
	VSCParameterControlView* parameterControlView;
	
	boost::bimap<VSCSParameter::Key, NSInteger> paramKeyIndexMap;

}

@property (nonatomic, retain) VSCParameterControlView* parameterControlView;

-(void) customInit;
-(void) createParameterControlView;



@end
