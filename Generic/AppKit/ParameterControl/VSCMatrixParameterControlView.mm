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



-(std::set<VSCSParameter::Key>&) parameterKeys {
	return parameterKeys;
}

-(boost::bimap<VSCSParameter::Key, NSUInteger>&) parameterKeyIndexBimap {
	return parameterKeyIndexBimap;
}

@end


@implementation VSCParameterControlView (Private)

-(void) customInit {
	
	self.centerSpacing = 10.0;
	
	self.scrollView = [[[NSScrollView alloc] initWithFrame:self.bounds] autorelease];
	[self addSubview:scrollView];
	
	self.controllerCellPrototype = [[[NSSliderCell alloc] init] autorelease];
	self.labelCellPrototype = [[NSCell alloc] initTextCell:@"No Parameter"];
	
}

#pragma mark Create/Destroy Matrices



-(void) createMatrices {
	
	[self destroyMatrices];
	
	NSInteger numberOfParameters = parameterKeys.size();
	
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

#pragma mark Parameter Labels/Values/Ranges

-(void) updateParameterLabels {
	NSInteger numberOfParameters = [self numberOfParameters];
	for (NSInteger i = 0; i < numberOfParameters; i++) {
		VSCSParameter::Key key = [self keyForParameterAtIndex:i];
		NSString* label = [VSCSoundApple labelForParameterWithKey:key];
		[parameterControlView setLabel:label forParameterAtIndex:i];
	}
}

-(void) updateParameterRanges {
	NSInteger numberOfParameters = [self numberOfParameters];
	for (NSInteger i = 0; i < numberOfParameters; i++) {
		VSCSParameter::Key key = [self keyForParameterAtIndex:i];
		NSString* label = [VSCSoundApple labelForParameterWithKey:key];
		[parameterControlView setLabel:label forParameterAtIndex:i];
	}
}

#pragma mark Parameter Key/Index

-(VSCSParameter::Key) keyForParameterAtIndex:(NSInteger)index {
	ParamKeyIndexBiMap::right_const_iterator right_iter = paramKeyIndexMap.right.find(index);
	// couldn't find the index in the bimap...
	if (right_iter == paramKeyIndexMap.right.end()) 
		return VSCSParameter::KeyNone;
	return right_iter->second;	
}

-(NSInteger) indexForParameterWithKey:(VSCSParameter::Key)key {
	ParamKeyIndexBiMap::left_const_iterator left_iter = paramKeyIndexMap.left.find(key);
	// couldn't find the key in the bimap...
	if (left_iter == paramKeyIndexMap.left.end()) 
		return -1;
	return left_iter->second;
}

#pragma mark NSCell Accessors and Utility 

-(NSCell*) controllerCellForParameterWithKey:(VSCSParameter::Key k) {
	NSInteger index = [self indexForParameterWithKey:k];
	return [controllerMatrix cellAtRow:index column:0];
}

-(NSCell*) numericCellForParameterWithKey:(VSCSParameter::Key k) {
	NSInteger index = [self indexForParameterWithKey:k];
	return [numericMatrix cellAtRow:index column:0];
}

-(NSCell*) labelCellForParameterWithKey:(VSCSParameter::Key k) {
	NSInteger index = [self indexForParameterWithKey:k];
	return [labelMatrix cellAtRow:index column:0];
}



@end
