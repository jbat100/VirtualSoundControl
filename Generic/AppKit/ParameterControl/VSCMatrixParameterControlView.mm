//
//  VSCParameterControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCMatrixParameterControlView.h"
#import "VSCMatrixParameterControlView+Private.h"
#import "VSCException.h"

@implementation VSCMatrixParameterControlView

@synthesize scrollView;
@synthesize controllerMatrix, labelMatrix, numericMatrix;
@synthesize controllerCellPrototype, labelCellPrototype, numericCellPrototype;
@synthesize spacing;

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


-(void) createInterface {
	
    [self createMatrices];
	
    [controllerMatrix sizeToCells];
    [labelMatrix sizeToCells];
	
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
	NSCell* labelCell = [self labelCellForParameterWithKey:k];
	[labelCell setStringValue:label];
}

-(void) setRange:(VSCSParameter::ValueRange)valueRange forParameterKey:(VSCSParameter::Key)k {
	[super setRange:valueRange forParameterKey:k];
	NSActionCell* controllerCell = [self controllerCellForParameterWithKey:k];
	if ([controllerCell isKindOfClass:[NSSliderCell class]]) {
		NSSliderCell* sliderCell = (NSSliderCell*)controllerCell;
		[sliderCell setMinValue:valueRange.first];
		[sliderCell setMaxValue:valueRange.second];
	}
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


#pragma mark - NSCell Accessors and Utility 

-(NSActionCell*) controllerCellForParameterWithKey:(VSCSParameter::Key)k {
	NSInteger index = [self indexForParameterWithKey:k];
	return [controllerMatrix cellAtRow:index column:0];
}

-(NSCell*) numericCellForParameterWithKey:(VSCSParameter::Key)k {
	NSInteger index = [self indexForParameterWithKey:k];
	return [numericMatrix cellAtRow:index column:0];
}

-(NSCell*) labelCellForParameterWithKey:(VSCSParameter::Key)k {
	NSInteger index = [self indexForParameterWithKey:k];
	return [labelMatrix cellAtRow:index column:0];
}

-(VSCSParameter::Key) parameterKeyForCell:(NSCell*)cell {
	
	NSInteger row, column;
	BOOL success;
	
	success = [controllerMatrix getRow:&row column:&column ofCell:cell];
	if (success)
		return [self keyForParameterAtIndex:row];
	
	success = [numericMatrix getRow:&row column:&column ofCell:cell];
	if (success)
		return [self keyForParameterAtIndex:row];
	
	success = [labelMatrix getRow:&row column:&column ofCell:cell];
	if (success)
		return [self keyForParameterAtIndex:row];
	
	throw VSCSInvalidArgumentException();
	
}

@end


@implementation VSCMatrixParameterControlView (Private)

-(void) customInit {
	
	self.spacing = 10.0;
	
	self.scrollView = [[[NSScrollView alloc] initWithFrame:self.bounds] autorelease];
	[self addSubview:scrollView];
	
	self.controllerCellPrototype = [[[NSSliderCell alloc] init] autorelease];
	self.labelCellPrototype = [[NSCell alloc] initTextCell:@"No Parameter"];
	
}

#pragma mark Create/Destroy Matrices



-(void) createMatrices {
	
	[self destroyMatrices];
	
	NSInteger numberOfParameters = keySet.size();
	
    CGRect labelMatrixFrame = NSMakeRect(0.0, 0.0, self.frame.size.width / 4.0, self.frame.size.height);
	self.labelMatrix = [[[NSMatrix alloc] initWithFrame:labelMatrixFrame 
												   mode:NSTrackModeMatrix 
											  prototype:self.labelCellPrototype 
										   numberOfRows:numberOfParameters 
										numberOfColumns:1] autorelease];
	[labelMatrix setAutoresizingMask: NSViewHeightSizable | NSViewWidthSizable];
	[scrollView addSubview:labelMatrix];
	
	
	CGRect controllerMatrixFrame = NSMakeRect(self.frame.size.width / 4.0, 0.0, 
											  self.frame.size.width / 2.0, self.frame.size.height);	
	self.controllerMatrix = [[[NSMatrix alloc] initWithFrame:controllerMatrixFrame 
														mode:NSTrackModeMatrix 
												   prototype:self.controllerCellPrototype 
												numberOfRows:numberOfParameters 
											 numberOfColumns:1] autorelease];	
	[controllerMatrix setAutoresizingMask: NSViewHeightSizable | NSViewWidthSizable];
	[scrollView addSubview:controllerMatrix];
	
	CGRect numericMatrixFrame = NSMakeRect(self.frame.size.width * (3.0 / 4.0), 0.0, 
											  self.frame.size.width / 4.0, self.frame.size.height);	
	
	self.numericMatrix = [[[NSMatrix alloc] initWithFrame:numericMatrixFrame 
														mode:NSTrackModeMatrix 
												   prototype:self.numericCellPrototype 
												numberOfRows:numberOfParameters 
											 numberOfColumns:1] autorelease];	
	[controllerMatrix setAutoresizingMask: NSViewHeightSizable | NSViewWidthSizable];
	[scrollView addSubview:numericMatrix];
	
	/*
	 *	Setup control tags so that the cells can be identified when sending messages
	 */
	[controllerMatrix setTarget:self];
	[controllerMatrix setAction:@selector(controllerCellCallback:)];

	
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




#pragma mark Controller Cell Callback 


-(void) controllerCellCallback:(NSActionCell*)sender {
	
	VSCSParameter::Key k = [self parameterKeyForCell:sender];
	
	if ([sender isKindOfClass:[NSSliderCell class]]) {
		double val = [(NSSliderCell*)sender doubleValue];
		[delegate parameterControlView:self changedParameterWithKey:k to:val];
	}
	
}

@end
