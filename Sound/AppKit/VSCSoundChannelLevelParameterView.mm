//
//  VSCSoundChannelLevelControlView.mm
//  SynthStation
//
//  Created by Jonathan Thorpe on 12/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSoundChannelLevelParameterView.h"
#import "VSCSoundParameterView+Private.h"

#import <boost/bimap.hpp>

typedef boost::bimap<VSCSParameter::Key, NSInteger> ParamKeyIndexMap;
typedef ParamKeyIndexMap::value_type ParamKeyIndexMapEntry;


@implementation VSCSoundChannelLevelParameterView

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    return self;
}

-(void) customInit {
	[super customInit];
	[self setMultiChannelElement:VSCSoundMultiChannelElementPtr()];
}

#pragma mark - Interface Update

/*
- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}
 */

-(void) updateParameterKeyIndexMap {
	
	/*
	 *	Set the parameters to represent the number of channels of the multi channel element
	 */
	paramKeyIndexMap.clear();
	for (int i = 0; i < multiChannelElement->getNumberOfChannels(); i++) {
		VSCSParameter::Key linearKey = VSCSParameter::keyForChannelIndex(i, false);
		paramKeyIndexMap.insert( ParamKeyIndexMapEntry(linearKey, i) );
        VSCSParameter::Key dBKey = VSCSParameter::keyForChannelIndex(i, true);
		paramKeyIndexMap.insert( ParamKeyIndexMapEntry(dBKey, i) );
	}
	
}

#pragma mark - Multi Channel Element C++ setter/getter

-(VSCSoundMultiChannelElementPtr) getMultiChannelElement {
	return multiChannelElement;
}

-(void) setMultiChannelElement:(VSCSoundMultiChannelElementPtr)_multiChannelElement {
	
	multiChannelElement = _multiChannelElement;
	
	
	[self createParameterControlInterface];
	
}

-(double) doubleValueForParameterWithKey:(VSCSParameter::Key)key {
	return 0.0;
}
	


@end
