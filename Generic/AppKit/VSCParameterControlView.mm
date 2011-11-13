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
@synthesize controllerMatrix, labelMatrix;
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

-(void) createInterfaceForParameterCount:(NSUInteger)index 
							 withOptions:(VSCParameterControlOptions)options {
	
    [self createMatrices];
	
    [controllerMatrix sizeToCells];
    [labelMatrix sizeToCells];
	
    
    [self setNeedsDisplay:YES];
	
	
	
}

-(void) setVSCSFloat:(VSCSFloat)f forParameterAtIndex:(NSUInteger)index {
	
}

-(void) setLabel:(NSString*)l forParameterAtIndex:(NSUInteger)index {
	
}

-(VSCSFloat) getParameterAtIndex:(NSUInteger)index {
	
}

@end


@implementation VSCParameterControlView (Private)

-(void) customInit {
	
	self.centerSpacing = 10.0;
	
	self.scrollView = [[[NSScrollView alloc] initWithFrame:self.bounds] autorelease];
	[self addSubview:scrollView];
	
	self.controllerCellPrototype = [[[NSSliderCell alloc] init] autorelease];
	self.labelCellPrototype = [[NSCell alloc] initTextCell:@"No Parameter"];
	
	self.numeOfParameters = 0;
	self.parameterControlOptions = VSCParameterControlOptionNone;
	
}

#pragma mark Interface Helpers



-(void) createMatrices {
	
	NSInteger numberOfRows = numeOfParameters;
	
	CGRect controllerMatrixFrame = NSMakeRect(self.frame.size.width / 2.0, 0.0, 
											  self.frame.size.width / 2.0, self.frame.size.height);	
	self.controllerMatrix = [[[NSMatrix alloc] initWithFrame:controllerMatrixFrame 
														mode:NSTrackModeMatrix 
												   prototype:self.controllerCellPrototype 
												numberOfRows:numeOfParameters 
											 numberOfColumns:1] autorelease];	
	[scrollView addSubview:controllerMatrix];
	

    
    CGRect labelMatrixFrame = NSMakeRect(0.0, 0.0, self.frame.size.width / 2.0, self.frame.size.height);
	self.labelMatrix = [[[NSMatrix alloc] initWithFrame:labelMatrixFrame 
												   mode:NSTrackModeMatrix 
											  prototype:self.labelCellPrototype 
										   numberOfRows:numberOfRows 
										numberOfColumns:1] autorelease];
	
	[scrollView addSubview:labelMatrix];
	
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

-(NSSize) sizeOfString:(NSString*)s withFont:(NSCell*)cell {
	
	NSFont* font = [cell font];
	NSDictionary* attributes = [NSDictionary dictionaryWithObject:font forKey:NSFontAttributeName];
	
	return [s sizeWithAttributes:attributes];;
	
}

@end
