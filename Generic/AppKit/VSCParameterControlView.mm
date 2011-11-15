//
//  VSCParameterControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCParameterControlView.h"
#import "VSCParameterControlView+Private.h"

@implementation VSCParameterControlView

@synthesize numberOfParameters, parameterControlOptions;
@synthesize scrollView;
@synthesize controllerMatrix, labelMatrix, numericMatrix;
@synthesize controllerCellPrototype, labelCellPrototype;
@synthesize centerSpacing;
@synthesize delegate, dataSource;

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
	
	[scrollView release];
	[controllerMatrix release];
	[labelMatrix release];
	[controllerCellPrototype release];
	[labelCellPrototype release];
	
	[super dealloc];
}

/*

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}
 
 */


#pragma mark - VSCParameterControlViewProtocol

-(void) createInterfaceForParameterCount:(NSUInteger)count {
	
	VSCParameterControlOptions defaultOptions = 
	VSCParameterControlOptionDisplayLabel		| 
	VSCParameterControlOptionDisplayNumeric		|	
	VSCParameterControlOptionDisplaySliders;
	
	[self createInterfaceForParameterCount:count withOptions:defaultOptions];
	
}

-(void) createInterfaceForParameterCount:(NSUInteger)count 
							 withOptions:(VSCParameterControlOptions)options {
	
	self.numberOfParameters = count;
	self.parameterControlOptions = options;
	
    [self createMatrices];
	
    [controllerMatrix sizeToCells];
    [labelMatrix sizeToCells];
	
    [self setNeedsDisplay:YES];
	
}

-(void) setLowerLimit:(double)low higherLimit:(double)high forParameterAtIndex:(NSUInteger)index {
	NSActionCell* controllerCell = [self controllerCellAtIndex:index];
	if ([controllerCell isKindOfClass:[NSSliderCell class]]) {
		NSSliderCell* sliderCell = (NSSliderCell*)controllerCell;
		[sliderCell setMinValue:low];
		[sliderCell setMaxValue:high];
	}
}

-(void) setDoubleValue:(double)f forParameterAtIndex:(NSUInteger)index {
	NSActionCell* controllerCell = [self controllerCellAtIndex:index];
	NSCell* numericCell = [self numericCellAtIndex:index];
	[controllerCell setDoubleValue:f];
	[numericCell setDoubleValue:f];
	
}

-(void) setLabel:(NSString*)l forParameterAtIndex:(NSUInteger)index {
	NSCell* labelCell = [self labelCellAtIndex:index];
	[labelCell setStringValue:l];
}

-(double) getParameterAtIndex:(NSUInteger)index {
	NSActionCell* controllerCell = [self controllerCellAtIndex:index];
	double val = [controllerCell doubleValue];
}

@end


@implementation VSCParameterControlView (Private)

-(void) customInit {
	
	self.centerSpacing = 10.0;
	
	self.scrollView = [[[NSScrollView alloc] initWithFrame:self.bounds] autorelease];
	[self addSubview:scrollView];
	
	self.controllerCellPrototype = [[[NSSliderCell alloc] init] autorelease];
	self.labelCellPrototype = [[NSCell alloc] initTextCell:@"No Parameter"];
	
	self.numberOfParameters = 0;
	self.parameterControlOptions = VSCParameterControlOptionNone;
	
}

#pragma mark Interface Helpers



-(void) createMatrices {
	
	[self destroyMatrices];
	
    CGRect labelMatrixFrame = NSMakeRect(0.0, 0.0, self.frame.size.width / 4.0, self.frame.size.height);
	self.labelMatrix = [[[NSMatrix alloc] initWithFrame:labelMatrixFrame 
												   mode:NSTrackModeMatrix 
											  prototype:self.labelCellPrototype 
										   numberOfRows:numberOfRows 
										numberOfColumns:1] autorelease];
	[labelMatrix setAutoresizingMask: NSViewHeightSizable | NSViewWidthSizable];
	[scrollView addSubview:labelMatrix];
	
	
	CGRect controllerMatrixFrame = NSMakeRect(self.frame.size.width / 4.0, 0.0, 
											  self.frame.size.width / 2.0, self.frame.size.height);	
	self.controllerMatrix = [[[NSMatrix alloc] initWithFrame:controllerMatrixFrame 
														mode:NSTrackModeMatrix 
												   prototype:self.controllerCellPrototype 
												numberOfRows:numeOfParameters 
											 numberOfColumns:1] autorelease];	
	[controllerMatrix setAutoresizingMask: NSViewHeightSizable | NSViewWidthSizable];
	[scrollView addSubview:controllerMatrix];
	
	CGRect numericMatrixFrame = NSMakeRect(self.frame.size.width * (3.0 / 4.0), 0.0, 
											  self.frame.size.width / 4.0, self.frame.size.height);	
	
	self.numericMatrix = [[[NSMatrix alloc] initWithFrame:numericMatrixFrame 
														mode:NSTrackModeMatrix 
												   prototype:self.numericCellPrototype 
												numberOfRows:numeOfParameters 
											 numberOfColumns:1] autorelease];	
	[controllerMatrix setAutoresizingMask: NSViewHeightSizable | NSViewWidthSizable];
	[scrollView addSubview:numericMatrix];
	
}


-(void) destroyMatrices {
	
	if (self.controllerMatrix) {
		[controllerMatrix removeFromSuperview];
		self.controllerMatrix = nil;
	}
	
	if (self.labelMatrix) {
		[labelMatrix removeFromSuperview];
		self.labelMatrix = nil;
	}
	
	if (self.numericMatrix) {
		[numericMatrix removeFromSuperview];
		self.numericMatrix = nil;
	}
	
}


#pragma mark NSCell Accessors and Utility 

-(NSCell*) controllerCellAtIndex:(NSInteger)index {
	NSAssert([controllerMatrix numberOfRows] > index, @"Index out of bounds");
	return [controllerMatrix cellAtRow:index column:0];
}

-(NSCell*) labelCellAtIndex:(NSInteger)index {
	NSAssert([labelMatrix numberOfRows] > index, @"Index out of bounds");
	return [labelMatrix cellAtRow:index column:0];
}

-(NSCell*) numericCellAtIndex:(NSInteger)index {
	NSAssert([numericMatrix numberOfRows] > index, @"Index out of bounds");
	return [numericMatrix cellAtRow:index column:0];
}



@end
