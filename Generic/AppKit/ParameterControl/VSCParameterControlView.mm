//
//  VSCParameterControlView.mm
//  SynthStation
//
//  Created by Jonathan Thorpe on 27/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCParameterControlView.h"
#import "NSString+VSCAdditions.h"
#import "VSCException.h"

@implementation VSCParameterControlView

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    return self;
}

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}

-(void) createInterface {
	throw VSCSCalledPureVirtualFunctionException();
}

#pragma mark - Set/Get Parameter Value

/*
 *	Value/Label changes
 */
-(void) setDoubleValue:(double)f forParameterWithKey:(VSCSParameter::Key)key {
	throw VSCSCalledPureVirtualFunctionException();
}

-(double) getDoubleValueForParameterWithKey:(VSCSParameter::Key)key {
	throw VSCSCalledPureVirtualFunctionException();
}

#pragma mark - Add/Remove Parameter Key

-(void) addParameterKey:(VSCSParameter::Key)k {
	keySet.insert(k);
}

-(void) removeParameterKey:(VSCSParameter::Key)k {
	keySet.erase(k);
}

-(void) addParameterKeys:(VSCSParameter::KeySet)kSet {
	keySet.insert(kSet.begin(), kSet.end());
}

-(void) removeParameterKeys:(VSCSParameter::KeySet)kSet {
	keySet.erase(kSet.begin(), kSet.end());
}

#pragma mark - Parameter Labels 

-(NSString*) getLabelForParameterKey:(VSCSParameter::Key)k {
	VSCSParameter::KeyLabelMap::iterator labelIterator = keyLabelMap.find(k);
	if (labelIterator != keyLabelMap.end()) 
		return [NSString stringWithStdString:(labelIterator->second)];
	throw VSCSBadParameterException();
}

-(void) setLabel:(NSString*)label forParameterKey:(VSCSParameter::Key)k {
	keyLabelMap.insert(VSCSParameter::KeyLabelPair (k, [label stdString]));
}

-(void) setDefaultLabelForParameterKey:(VSCSParameter::Key)k {
	std::string label = VSCSParameter::sharedInstance().getLabelForParameterWithKey(k);
	[self setLabel:[NSString stringWithStdString:label] forParameterKey:k];
}

-(void) setDefaultLabelForAllParameterKeys {
	for (VSCSParameter::KeySet::iterator iter = keySet.begin(); iter != keySet.end(); iter++) {
		[self setDefaultLabelForParameterKey:(*iter)];
	}
}

#pragma mark - Parameter Ranges

-(void) setRange:(VSCSParameter::ValueRange)valueRange forParameterKey:(VSCSParameter::Key)k {
	keyRangeMap.insert(VSCSParameter::KeyRangePair (k, valueRange));
}

-(void) setDefaultRangeForParameterKey:(VSCSParameter::Key)k {
	VSCSParameter::ValueRange r = VSCSParameter::sharedInstance().getRangeForParameterWithKey(k);
	[self setRange:r forParameterKey:k];
}

-(void) setDefaultRangeForAllParameterKeys {
	for (VSCSParameter::KeySet::iterator iter = keySet.begin(); iter != keySet.end(); iter++) {
		[self setDefaultRangeForParameterKey:(*iter)];
	}
}


-(VSCSParameter::ValueRange) getRangeForParameterKey:(VSCSParameter::Key)k {
	VSCSParameter::KeyRangeMap::iterator rangeIterator = keyRangeMap.find(k);
	if (rangeIterator != keyRangeMap.end()) 
		return rangeIterator->second;
	throw VSCSBadParameterException();
}

#pragma mark - Const References For Outer Operations

-(const VSCSParameter::KeySet&) keySet {
	return keySet;
}

-(const VSCSParameter::KeyLabelMap&) keyLabelMap {
	return keyLabelMap;
}

-(const VSCSParameter::KeyRangeMap&) keyRangeMap {
	return keyRangeMap;
}


@end
