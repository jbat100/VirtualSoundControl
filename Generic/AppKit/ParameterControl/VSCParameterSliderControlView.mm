//
//  VSCParameterControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCParameterSliderControlView.h"
#import "VSCSingleParameterSliderControlView.h"
#import "NSString+VSCAdditions.h"
#import "VSCException.h"

@interface VSCParameterSliderControlView () 

@property (nonatomic, retain) NSNib* singleParameterSliderControlViewNib;
@property (nonatomic, retain) NSMutableArray* singleParameterSliderControlViews;

-(VSCSingleParameterSliderControlView*) singleParameterSliderControlViewForKey:(VSCSParameter::Key)key;

@end


@implementation VSCParameterSliderControlView

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
	self.singleParameterSliderControlViewNib = nil;
	self.singleParameterSliderControlViews = nil;
	[super dealloc];
}

/*

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}
 
 */


#pragma mark - VSCParameterControlViewProtocol


-(void) createInterface {
	
	NSArray* topLevelObjects = nil;
	
	self.singleParameterSliderControlViewNib = 
	[[NSNib alloc] initWithNibNamed:self.singleParameterSliderControlViewNibName bundle:nil];
	[singleParameterSliderControlViewNib instantiateNibWithOwner:self topLevelObjects:&topLevelObjects];
	
	VSCSingleParameterSliderControlView* v = [topLevelObjects objectAtIndex:0];
	
	NSSize s = v.frame.size;
	NSRect f = NSMakeRect(0.0, 0.0, s.width, s.height * keyList.size());
	self.bounds = f;
	
	for (VSCSParameter::KeyList::iterator it = keyList.begin(); it != keyList.end(); it++) {
		NSArray* topLevelObjects = nil;
		[singleParameterSliderControlViewNib instantiateNibWithOwner:self topLevelObjects:&topLevelObjects];
		v = [topLevelObjects objectAtIndex:0];
		VSCSParameter::Key k = *it;
		v.key = k;
		VSCSParameter::ValueRange r = VSCSParameter::sharedInstance().getRangeForParameterWithKey(k);
		v.valueRange = r;
		v.label = [NSString stringWithStdString:VSCSParameter::sharedInstance().getLabelForParameterWithKey(k)];
	}
    
	[self setNeedsDisplay:YES];
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

-(void) customInit {
	self.horizontalMargin = 10.0;
	self.verticalMargin = 10.0;
	self.singleParameterSliderControlViews = [NSMutableArray array];
	self.singleParameterSliderControlViewNibName = @"VSCParameterSliderControlView";
}

#pragma mark Create/Destroy Matrices

-(VSCSingleParameterSliderControlView*) singleParameterSliderControlViewForKey:(VSCSParameter::Key)key {
	assert(find(keyList.begin(), keyList.end(), key) != keyList.end());
	if (find(keyList.begin(), keyList.end(), key) == keyList.end()) return nil;
	for (VSCSingleParameterSliderControlView* v in singleParameterSliderControlViews) {
		VSCSParameter::Key vk = v.key;
		if (vk == key) return v;
	}
	return nil;
}


#pragma mark Controller Cell Callback 

-(void) object:(id)sender changedParameterWithKey:(VSCSParameter::Key)key to:(double)val {
	VSCSingleParameterSliderControlView* v = (VSCSingleParameterSliderControlView*)sender;
	[listener object:self changedParameterWithKey:key to:val];
}


@end
