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

@synthesize listener;

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

-(void) createDefaultInterface {
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
	if (find(keyList.begin(), keyList.end(), k) == keyList.end()) {
		keyList.push_back(k);
	}
}

-(void) removeParameterKey:(VSCSParameter::Key)k {
	if (find(keyList.begin(), keyList.end(), k) == keyList.end()) {
		keyList.erase(find(keyList.begin(), keyList.end(), k));
	}
}

-(void) addParameterKeys:(VSCSParameter::KeyList)kList {
	for (VSCSParameter::KeyList::iterator it = kList.begin(); it != kList.end(); it++) {
		[self addParameterKey:(*it)];
	}
}

-(void) removeParameterKeys:(VSCSParameter::KeyList)kList {
	for (VSCSParameter::KeyList::iterator it = kList.begin(); it != kList.end(); it++) {
		[self removeParameterKey:(*it)];
	}
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
	for (VSCSParameter::KeyList::iterator iter = keyList.begin(); iter != keyList.end(); iter++) {
		[self setDefaultLabelForParameterKey:(*iter)];
	}
}

#pragma mark - Parameter Ranges

-(VSCSParameter::ValueRange) getRangeForParameterKey:(VSCSParameter::Key)k {
	VSCSParameter::KeyRangeMap::iterator rangeIterator = keyRangeMap.find(k);
	if (rangeIterator != keyRangeMap.end()) 
		return rangeIterator->second;
	throw VSCSBadParameterException();
}

-(void) setRange:(VSCSParameter::ValueRange)valueRange forParameterKey:(VSCSParameter::Key)k {
	keyRangeMap.insert(VSCSParameter::KeyRangePair (k, valueRange));
}

-(void) setDefaultRangeForParameterKey:(VSCSParameter::Key)k {
	VSCSParameter::ValueRange r = VSCSParameter::sharedInstance().getRangeForParameterWithKey(k);
	[self setRange:r forParameterKey:k];
}

-(void) setDefaultRangeForAllParameterKeys {
	for (VSCSParameter::KeyList::iterator iter = keyList.begin(); iter != keyList.end(); iter++) {
		[self setDefaultRangeForParameterKey:(*iter)];
	}
}

#pragma mark - Const References For Outer Operations

-(BOOL) controlsParameterWithKey:(VSCSParameter::Key)key {
    
    if (std::find(keyList.begin(), keyList.end(), key) != keyList.end()) {
        return YES;
    }
    
    return NO;
    
}

-(const VSCSParameter::KeyList&) keyList {
	return keyList;
}

-(const VSCSParameter::KeyLabelMap&) keyLabelMap {
	return keyLabelMap;
}

-(const VSCSParameter::KeyRangeMap&) keyRangeMap {
	return keyRangeMap;
}


@end
