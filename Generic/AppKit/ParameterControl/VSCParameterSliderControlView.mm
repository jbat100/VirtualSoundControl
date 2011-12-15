//
//  VSCParameterControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCParameterSliderControlView.h"
#import "VSCSingleParameterSliderControlView.h"
#import "VSCException.h"

@interface VSCParameterSliderControlView () {
	
	NSNib* singleParameterSliderControlViewNib;
	
	NSMutableArray* singleParameterSliderControlViews;
	
}

@property (nonatomic, retain) NSNib* singleParameterSliderControlViewNib;

@property (nonatomic, retain) NSMutableArray* singleParameterSliderControlViews;

-(VSCSingleParameterSliderControlView*) singleParameterSliderControlViewForKey:(VSCSParameter::Key)key;

@end


@implementation VSCMatrixParameterControlView

@synthesize singleParameterSliderControlViewNib;
@synthesize horizontalMargin;
@synthesize verticalMargin;

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
		[self customInit];
    }
    return self;
}

-(id) initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    if (self) {
        // Initialization code here.
		[self customInit];
    }
    return self;
}


-(void) dealloc {
	
	
	[super dealloc];
}

/*

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}
 
 */


#pragma mark - VSCParameterControlViewProtocol


-(void) createInterface {
	
	self.singleParameterSliderControlViewNib = 
	[[NSNib alloc] initWithNibNamed:self.singleParameterSliderControlViewNibName bundle:nil];
	
	VSCSParameter::KeyIndexBimap::right_const_iterator it;
	
	for (it = parameterKeyIndexBimap.right.begin(); it != parameterKeyIndexBimap.right.(); it++) {
		
	}
	
    [self setNeedsDisplay:YES];
	
}


-(void) setparameterKeyIndexBimap:(VSCSParameter::KeyIndexBimap)keyIndexBymap {
	parameterKeyIndexBimap = keyIndexBymap;
}

-(const VSCSParameter::KeyIndexBimap&) parameterKeyIndexBimap {
	return parameterKeyIndexBimap;
}

-(void) setLabel:(NSString*)label forParameterKey:(VSCSParameter::Key)k {
	[super setLabel:label forParameterKey:k];
	[[self singleParameterSliderControlViewForKey:k] setLabel:label];
}

-(void) setRange:(VSCSParameter::ValueRange)valueRange forParameterKey:(VSCSParameter::Key)k {
	[super setRange:valueRange forParameterKey:k];
	[[self singleParameterSliderControlViewForKey:k] setValueRange:valueRange];
}

#pragma mark Parameter Key/Index

-(VSCSParameter::Key) keyForParameterAtIndex:(NSInteger)index {
	VSCSParameter::KeyIndexBimap::right_const_iterator right_iter = parameterKeyIndexBimap.right.find(index);
	// couldn't find the index in the bimap...
	if (right_iter == parameterKeyIndexBimap.right.end()) 
		throw VSCSInvalidArgumentException();
	return right_iter->second;	
}

-(NSInteger) indexForParameterWithKey:(VSCSParameter::Key)key {
	VSCSParameter::KeyIndexBimap::left_const_iterator left_iter = parameterKeyIndexBimap.left.find(key);
	// couldn't find the key in the bimap...
	if (left_iter == parameterKeyIndexBimap.left.end()) 
		throw VSCSInvalidArgumentException();
	return left_iter->second;
}



-(void) customInit {
	
	self.horizontalMargin = 10.0;
	self.verticalMargin = 10.0;
	
	self.singleParameterSliderControlViews = [NSMutableArray array];
	
	self.singleParameterSliderControlViewNibName = @"VSCParameterSliderControlView";
	
}

#pragma mark Create/Destroy Matrices

-(VSCSingleParameterSliderControlView*) singleParameterSliderControlViewForKey:(VSCSParameter::Key)key {
	assert(keySet.find(key) != keySet.end())
	if (keySet.find(key) == keySet.end()) return nil;
	NSInteger viewIndex = [self indexForParameterWithKey:key];
	assert([singleParameterSliderControlViews count] > viewIndex);
	if ([singleParameterSliderControlViews count] <= viewIndex) return nil;
	VSCSingleParameterSliderControlView* v = [singleParameterSliderControlViews objectAtIndex:viewIndex];
	assert(v.key == key)
	if (v.key != key) return nil;
	return v;
}


#pragma mark Controller Cell Callback 


-(void) controllerCellCallback:(NSActionCell*)sender {
	
	VSCSParameter::Key k = [self parameterKeyForCell:sender];
	
	if ([sender isKindOfClass:[NSSliderCell class]]) {
		double val = [(NSSliderCell*)sender doubleValue];
		[delegate parameterControlView:self changedParameterWithKey:k to:val];
	}
	
}

@end
