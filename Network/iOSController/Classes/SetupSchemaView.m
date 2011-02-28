//
//  SetupSchemaView.m
//  iOSController
//
//  Created by Jonathan Thorpe on 28/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "SetupSchemaView.h"

#import "ControlCenterSetup.h"

#import "ControlViewSetup.h"
#import "TouchControlViewSetup.h"
#import "SliderControlViewSetup.h"
#import "ButtonControlViewSetup.h"


@implementation SetupSchemaView

@synthesize setup, viewOutlineWidth, viewOutlineColor;
@synthesize touchViewBackgroundColor, sliderViewBackgroundColor, buttonViewBackgroundColor;


- (id)initWithFrame:(CGRect)frame {
    
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code.
		
		viewOutlineWidth = 2;
		
		self.viewOutlineColor = [UIColor blackColor];
		
		self.touchViewBackgroundColor = [UIColor grayColor];
		self.sliderViewBackgroundColor = [UIColor grayColor];
		self.buttonViewBackgroundColor = [UIColor grayColor];
		
    }
    return self;
}


// Only override drawRect: if you perform custom drawing.
// An empty implementation adversely affects performance during animation.
- (void)drawRect:(CGRect)rect {
    // Drawing code.
	
	CGSize size = self.bounds.size;
	size.width -= viewOutlineWidth;
	size.height -= viewOutlineWidth;
	NSInteger offset = viewOutlineWidth / 2;
	
	CGContextRef ctx = UIGraphicsGetCurrentContext();
	
	for (ControlViewSetup* controlViewSetup in setup.controlViewSetups) {
		
		CGRect nf = controlViewSetup.normalisedFrame;
		
		// Control view outline
		
		CGMutablePathRef controlViewOutlinePath = CGPathCreateMutable(); 
		
		CGPathMoveToPoint(controlViewOutlinePath, NULL, 
						  (size.width * nf.origin.x) + offset, 
						  (size.height * nf.origin.y) + offset);
		
		CGPathAddLineToPoint(controlViewOutlinePath, NULL, 
							 (size.width * (nf.origin.x + nf.size.width)) + offset, 
							 (size.height * nf.origin.y) + offset); 
		
		
		CGPathAddLineToPoint(controlViewOutlinePath, NULL, 
							 (size.width * (nf.origin.x + nf.size.width)) + offset, 
							 (size.height * (nf.origin.y + nf.size.height)) + offset); 
		
		CGPathAddLineToPoint(controlViewOutlinePath, NULL, 
							 (size.width * nf.origin.x) + offset, 
							 (size.height * (nf.origin.y + nf.size.height)) + offset); 
		
		CGPathAddLineToPoint(controlViewOutlinePath, NULL, 
							 (size.width * nf.origin.x) + offset, 
							 (size.height * nf.origin.y) + offset); 
		
		CGPathCloseSubpath(controlViewOutlinePath); 
		
		
		CGContextSetLineWidth(ctx, viewOutlineWidth);
		CGContextSetStrokeColorWithColor(ctx, viewOutlineColor.CGColor);
		
		if ([controlViewSetup isKindOfClass:[TouchControlViewSetup class]]) {
			CGContextSetFillColorWithColor(ctx, touchViewBackgroundColor.CGColor);  
		}
		
		else if ([controlViewSetup isKindOfClass:[SliderControlViewSetup class]]) {
			CGContextSetFillColorWithColor(ctx, sliderViewBackgroundColor.CGColor);  
		}
		
		else if ([controlViewSetup isKindOfClass:[ButtonControlViewSetup class]]) {
			CGContextSetFillColorWithColor(ctx, buttonViewBackgroundColor.CGColor);  
		}
		
		CGContextAddPath(ctx, controlViewOutlinePath);
		
		CGPathRelease(controlViewOutlinePath);
		
		CGContextFillPath(ctx);
		
	}
}


- (void)dealloc {
    [super dealloc];
}


@end
