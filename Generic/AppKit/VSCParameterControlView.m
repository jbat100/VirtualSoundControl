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
@synthesize centerSpacing;
@synthesize delegate;

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
	
	[parameterKeys release];
	[scrollView release];
	[controllerMatrix release];
	[labelMatrix release];
	[controllerCellPrototype release];
	[labelCellPrototype release];
	
	[super dealloc];
}

-(void) customInit {
	
	self.centerSpacing = 10.0;
	
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

-(void) removeAllParameters {
	
	[parameterKeys removeAllObjects];
	
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
	
	[scrollView addSubview:controllerMatrix];
	
}

-(void) createLabelMatrix {
	
	self.labelMatrix = [[[NSMatrix alloc] initWithFrame:labelMatrixFrame 
												   mode:NSTrackModeMatrix 
											  prototype:self.labelCellPrototype 
										   numberOfRows:0 
										numberOfColumns:1] autorelease];
	
	[scrollView addSubview:labelMatrix];
	
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

-(void) reloadParameterForKey:(NSString*)key {
	
	NSInteger rowIndex = [delegate parameterControlView:self displayIndexForParameterWithKey:key];
	
	NSAssert(rowIndex < [parameterKeys count], @"Invalid row index");
	
	if (!delegate)
		return;
	
	if (rowIndex >= [parameterKeys count])
		rowIndex = [parameterKeys count]-1;
	
	NSString* displayString = [delegate parameterControlView:self displayStringForParameterWithKey:key];
	NSCell* labelCell = [labelMatrix cellAtRow:rowIndex column:0];
	[labelCell setStringValue:displayString];
	
	NSCell* controllerCell = [controllerMatrix cellAtRow:rowIndex column:0];
	SEL fetchSelector = [delegate parameterControlView:self fetchSelectorForParameterWithKey:key];
	
	if (fetchSelector == @selector(parameterControlView:stringForParameterWithKey:)) {
		NSString* val = [delegate parameterControlView:self stringForParameterWithKey:key];
		[controllerCell setStringValue:val];
	}
	else if (fetchSelector == @selector(parameterControlView:floatForParameterWithKey:)) {
		float val = [delegate parameterControlView:self floatForParameterWithKey:key];
		[controllerCell setFloatValue:val];
	}
	else if (fetchSelector == @selector(parameterControlView:doubleForParameterWithKey:)) {
		double val = [delegate parameterControlView:self doubleForParameterWithKey:key];
		[controllerCell setDoubleValue:val];
	}
	
}

-(void) updateInterface {
	
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
	
	if (!delegate)
		return;
	
	for (NSString* key in parameterKeys) {
		[self reloadParameterForKey:key];
	}
	
}

-(void) resetInterface {
	
	[self destroyMatrices];
	[self createMatrices];
	[self updateInterface];
	
}

#pragma mark Interface Helpers

-(NSSize) sizeOfString:(NSString*)s inCell:(NSCell*)cell {
	
	NSFont* font = [cell font];
	NSDictionary* attributes = [NSDictionary dictionaryWithObject:font forKey:NSFontAttributeName]];
	 
	return [s sizeWithAttributes:attributes];;
	
}


@end
