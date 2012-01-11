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

-(void) customInit;
-(VSCSingleParameterSliderControlView*) singleParameterSliderControlViewForKey:(VSCSParameter::Key)key;

@end


@implementation VSCParameterSliderControlView

@synthesize singleParameterSliderControlViewNibName;
@synthesize singleParameterSliderControlViewNib;
@synthesize singleParameterSliderControlViews;
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

-(void) customInit {
	self.horizontalMargin = 10.0;
	self.verticalMargin = 10.0;
	self.singleParameterSliderControlViews = [NSMutableArray array];
	self.singleParameterSliderControlViewNibName = @"VSCSingleParameterSliderControlView";
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
    
    // First, clear all the old single parameter slider control view
    NSArray* subviews = [self subviews];
    for (NSView* subview in subviews) {
        if ([subview isKindOfClass:[VSCSingleParameterSliderControlView class]]) {
            [subview removeFromSuperview];
        }
    }
    
	NSArray* topLevelObjects = nil;
	self.singleParameterSliderControlViewNib = 
	[[NSNib alloc] initWithNibNamed:self.singleParameterSliderControlViewNibName bundle:nil];
	[singleParameterSliderControlViewNib instantiateNibWithOwner:self topLevelObjects:&topLevelObjects];
    
    NSLog(@"Top level objects: %@", topLevelObjects);
	
	VSCSingleParameterSliderControlView* v = nil;
    for (NSObject* o in topLevelObjects) {
        if ([o isKindOfClass:[VSCSingleParameterSliderControlView class]]) {
            v = (VSCSingleParameterSliderControlView*)o;
        }
    }
    
    NSAssert(v, @"Could not load interface from nib");
    
    NSLog(@"Loaded %@ with frame %@", v, NSStringFromRect(v.frame));
	
	//NSSize s = self.frame.size;
	
    CGFloat currentVerticalOffset = 0.0;
	for (VSCSParameter::KeyList::iterator it = keyList.begin(); it != keyList.end(); it++) {
        
        NSLog(@"Making slider view for ");
        std::cout << VSCSParameter::sharedInstance().getLabelForParameterWithKey(*it) << std::endl;
        
        // build single slider view from nib 
		NSArray* topLevelObjects = nil;
		[singleParameterSliderControlViewNib instantiateNibWithOwner:self topLevelObjects:&topLevelObjects];
        for (NSObject* o in topLevelObjects) {
            if ([o isKindOfClass:[VSCSingleParameterSliderControlView class]]) {
                v = (VSCSingleParameterSliderControlView*)o;
            }
        }
        NSAssert(v, @"Could not load interface from nib");
        
		VSCSParameter::Key k = *it;
		v.key = k;
		VSCSParameter::ValueRange r = VSCSParameter::sharedInstance().getRangeForParameterWithKey(k);
		v.valueRange = r;
		v.label = [NSString stringWithStdString:VSCSParameter::sharedInstance().getLabelForParameterWithKey(k)];
        
        NSRect f = NSMakeRect(0.0, currentVerticalOffset, v.frame.size.width, v.frame.size.height);
        v.frame = f;
        
        NSLog(@"Adding %@ (frame: %@) to %@ (frame %@), currentVerticalOffset %f", 
              v, NSStringFromRect(v.frame), self, NSStringFromRect(self.frame), currentVerticalOffset);
        
        [self addSubview:v];
        
        currentVerticalOffset += v.frame.size.height;
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
	//VSCSingleParameterSliderControlView* v = (VSCSingleParameterSliderControlView*)sender;
	[listener object:self changedParameterWithKey:key to:val];
}


@end
