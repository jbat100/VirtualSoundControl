//
//  VSCParameterControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCParameterControlView.h"

@interface VSCParameterControlView () 

-(void) createControllerMatrix;
-(void) createLabelMatrix;

@end

@implementation VSCParameterControlView

@synthesize parameterKeys, scrollView;
@synthesize controllerMatrix, labelMatrix;
@synthesize controllerCellPrototype, labelCellPrototype;

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
		[self customInit];
    }
    return self;
}

-(void) dealloc {
	
	[parameterKeys release];
	[scrollView release];
	[controllerMatrix release];
	[labelMatrix release];
	[controllerCellPrototype release];
	[labelCellPrototype release];
	
	[super dealloc];
}

-(void) customInit {
	
	self.parameterKeys = [NSMutableSet setWithCapacity:10];
	self.scrollView = [[[NSScrollView alloc] initWithFrame:self.bounds] autorelease];
	[self addSubview:scrollView];
	
	self.controllerCellPrototype = [[[NSSliderCell alloc] init] autorelease];
	self.labelCellPrototype = [[NSCell alloc] initTextCell:@"No Parameter"];
	
	CGRect labelMatrixFrame = NSMakeRect(0.0, 0.0, self.frame.size.width / 2.0, self.frame.size.height);
	
}

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}

#pragma mark Adding/Removing Parameters

-(void) addParameterWithKey:(NSString*)key {
	
	[parameterKeys addObject:key];
	
	[self updateInterface];
	
}

-(void) removeParameterWithKey:(NSString*)key {
	
	[parameterKeys removeObject:key];
	
	[self updateInterface];
	
}

-(NSSet*) parameterKeys {
	
	return [[parameterKeys retain] autorelease];
	
}

#pragma mark Matrix Creation/Destruction 

-(void) createControllerMatrix {
	
	CGRect controllerMatrixFrame = NSMakeRect(self.frame.size.width / 2.0, 0.0, 
											  self.frame.size.width / 2.0, self.frame.size.height);
	
	self.controllerMatrix = [[[NSMatrix alloc] initWithFrame:controllerMatrixFrame 
														mode:NSTrackModeMatrix 
												   prototype:self.controllerCellPrototype 
												numberOfRows:0 
											 numberOfColumns:1] autorelease];	
	
	[self addSubview:controllerMatrix];
	
}

-(void) createLabelMatrix {
	
	self.labelMatrix = [[[NSMatrix alloc] initWithFrame:labelMatrixFrame 
												   mode:NSTrackModeMatrix 
											  prototype:self.labelCellPrototype 
										   numberOfRows:0 
										numberOfColumns:1] autorelease];
	
	[self addSubview:labelMatrix];
	
}

-(void) createMatrices {
	
	[self createControllerMatrix];
	[self createLabelMatrix];
	
}

-(void) destroyControllerMatrix {
	
	if (self.controllerMatrix) {
		[controllerMatrix removeFromSuperview];
		self.controllerMatrix = nil;
	}
	
}

-(void) destroyLabelMatrix {
	
	if (self.labelMatrix) {
		[labelMatrix removeFromSuperview];
		self.labelMatrix = nil;
	}
	
}

-(void) destroyMatrices {
	
	[self destroyControllerMatrix];
	[self destroyLabelMatrix];
	
}

#pragma mark Update/Reset Interface

-(void) updateInterface {
	
	if (!controllerMatrix)
		[self createLabelMatrix];
	if (!labelMatrix)
		[self labelMatrix];
	
	if ([controllerMatrix numberOfRows] != [parameterKeys count]) {
		
		if ([controllerMatrix numberOfRows] < [parameterKeys count]) {
			while ([controllerMatrix numberOfRows] < [parameterKeys count]) {
				[controllerMatrix addRow];
			}
		}
		
		else if ([controllerMatrix numberOfRows] > [parameterKeys count]) {
			while ([controllerMatrix numberOfRows] > [parameterKeys count]) {
				[controllerMatrix removeRow:0];
			}
		}
		
	}
	
	
}

-(void) resetInterface {
	
	[self destroyMatrices];
	[self updateInterface];
	
}


@end
