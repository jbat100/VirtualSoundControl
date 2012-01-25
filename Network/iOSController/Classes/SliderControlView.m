//
//  SliderControlView.m
//  iOSController
//
//  Created by Jonathan Thorpe on 09/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "SliderControlView.h"
#import "SliderControlViewSetup.h"
#import "SliderSetup.h"
#import "Definitions.h"


@implementation SliderControlView

- (id)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
		sliders = [[NSMutableArray alloc] initWithCapacity:MAX_SLIDERS];
		changedSliders = [[NSMutableArray alloc] initWithCapacity:MAX_SLIDERS];
    }
    return self;
}

- (void)drawRect:(CGRect)dirtyRect {
    // Drawing code here.
}

-(void) dealloc {
	
	[sliders release];
	[changedSliders release];
	
	[super dealloc];
	
}

#pragma mark -
#pragma mark Slider Setups

-(void) updateSliderSetups {
	
	NSObject<SliderControlViewSetup>* castedSetup = (NSObject<SliderControlViewSetup>*) setup;
	
	if ([castedSetup.sliderSetups count] != [sliders count]) {
		[NSException raise:@"Unexpected slider count" format:@"Unmatched count for sliders: %@ \nand setups %@", 
		 sliders, castedSetup.sliderSetups];
	}
	
	NSInteger sliderCount = 0;
	for (SliderSetup* sliderSetup in castedSetup.sliderSetups) {
		sliderSetup.value = [(UISlider*)[sliders objectAtIndex:sliderCount] value];
		sliderCount++;
	}
	
}

-(void) applySetup {
	
	[super applySetup];
	
	// avoids annoying warnings
	NSObject<SliderControlViewSetup>* castedSetup = (NSObject<SliderControlViewSetup>*) setup;
	
	for (UISlider* slider in sliders) {
		if ([slider isKindOfClass:[UISlider class]] && [slider superview] == self) 
			[slider removeFromSuperview];
	}
	
	while ([sliders count] < [setup.sliderSetups count]) {
		slider = [[UISlider alloc] init];
		slider.autoresizingMask = UIViewAutoresizingFlexibleWidth | UIViewAutoresizingFlexibleHeight;
		[slider addTarget:self action:@selector(sliderValueChanged:) forControlEvents:UIControlEventValueChanged];
		[sliders addObject:slider];
		[slider release];
	}
	
	CGSize s = self.frame.size;
	
	NSInteger verticalStep = (NSInteger)(s.height / [setup.sliderSetups count]);
	NSInteger verticalOffset = (NSInteger)(verticalStep / 2);
	NSInteger horizontalOffset = (NSInteger)(s.width / 8);
	
	CGRect sliderFrame = CGRectMake(horizontalOffset, verticalOffset, 
									s.width - (horizontalOffset*2), s.height - (horizontalOffset*2))
	
	NSInteger sliderCount = 0;
	
	for (SliderSetup* sliderSetup in castedSetup.sliderSetups) {
		
		if (![sliderSetup isKindOfClass:[SliderSetup class]])
			[NSException raise:@"Invalid slider setup" format:@"%@ should be a SliderSetup instance", sliderSetup];
		
		UISlider* slider = [sliders objectAtIndex:sliderCount];
		slider.frame = sliderFrame;
		[self addSubview:slider];
		
		slider.minimumValue = sliderSetup.minValue;
		slider.maximumValue = sliderSetup.maxValue;
		slider.value = slider.value;
		
		sliderFrame.origin.y += verticalStep;
		sliderCount++;
		
	}

	
	if (castedSetup.sliderOrientation == SliderOrientationVertical) {
		
	}
	
	else if (castedSetup.sliderOrientation == SliderOrientationHorizontal) {
		
	}
	
}

#pragma mark -
#pragma mark Slider Change Callback 

-(void) sliderValueChanged:(id)sender {
	changedSinceLastUpdate = YES;
	[changedSliders addObject:(UISlider*)sender];
	
}

-(void) clearChangedSliders {
	[changedSliders removeAllObjects];
}

#pragma mark -
#pragma mark Custom Setup Setter

-(void) setSetup:(NSObject*)s {
	
	[super setSetup:s];
	
	if (![s conformsToProtocol:@protocol(SliderControlViewSetup)]) {
		[NSException raise:@"Invalid setup object" format:@"%@ does not conform to <SliderControlViewSetup>", s];
		setup = nil;
	}
	
}


@end
