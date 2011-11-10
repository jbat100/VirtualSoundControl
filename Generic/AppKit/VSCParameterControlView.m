//
//  VSCParameterControlView.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCParameterControlView.h"

@implementation VSCParameterControlView

@synthesize parameterKeys, scrollView;
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
	
	self.scrollView = [[[NSScrollView alloc] initWithFrame:self.bounds] autorelease];
	[self addSubview:scrollView];
	
	self.controllerCellPrototype = [[[NSSliderCell alloc] init] autorelease];
	self.labelCellPrototype = [[NSCell alloc] initTextCell:@"No Parameter"];
	
	CGRect labelMatrixFrame = NSMakeRect(0.0, 0.0, self.frame.size.width / 2.0, self.frame.size.height);
	
}

/*

- (void)drawRect:(NSRect)dirtyRect {
    // Drawing code here.
}
 
 */


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
    
    CGRect labelMatrixFrame = NSMakeRect(0.0, 0.0, self.frame.size.width / 2.0, self.frame.size.height);
	
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

-(void) updateInterface {
    
    NSInteger parameterCount = [dataSource parameterControlViewNumberOfParameters:self];
	
	if ([controllerMatrix numberOfRows] != parameterCount) {
		
		if ([controllerMatrix numberOfRows] < parameterCount) {
			while ([controllerMatrix numberOfRows] < parameterCount) {
				[controllerMatrix addRow];
			}
		}
		
		else if ([controllerMatrix numberOfRows] > parameterCount) {
			while ([controllerMatrix numberOfRows] > parameterCount) {
				[controllerMatrix removeRow:0];
			}
		}
		
	}
    
    if ([labelMatrix numberOfRows] != parameterCount) {
		
		if ([labelMatrix numberOfRows] < parameterCount) {
			while ([labelMatrix numberOfRows] < parameterCount) {
				[labelMatrix addRow];
			}
		}
		
		else if ([labelMatrix numberOfRows] > parameterCount) {
			while ([labelMatrix numberOfRows] > parameterCount) {
				[labelMatrix removeRow:0];
			}
		}
		
	}
    
    [controllerMatrix sizeToCells];
    [labelMatrix sizeToCells];
    
	
	if (!delegate)
		return;
	
	[self reloadAllParameters];
    
    [self setNeedsDisplay:YES];
	
}

-(void) resetInterface {
	
	[self destroyMatrices];
	[self createMatrices];
	[self updateInterface];
	
}

#pragma mark - Reload parameter values

-(void) reloadParameterValueAtIndexPath:(NSIndexPath*)indexPath {
    
    if (!dataSource)
		return;
    
    NSInteger rowIndex = [indexPath indexAtPosition:0];
	
	NSAssert(rowIndex < parameterCount, @"Invalid row index");
	
	if (rowIndex >= parameterCount)
		rowIndex = parameterCount-1;
	
	
	NSCell* controllerCell = [controllerMatrix cellAtRow:rowIndex column:0];
	SEL fetchSelector = [dataSource parameterControlView:self fetchSelectorForParameterAtIndexPath:indexPath];
	
	if (fetchSelector == @selector(parameterControlView:stringForParameterAtIndexPath:)) {
		NSString* val = [dataSource parameterControlView:self stringForParameterAtIndexPath:indexPath];
		[controllerCell setStringValue:val];
	}
	else if (fetchSelector == @selector(parameterControlView:floatForParameterAtIndexPath:)) {
		float val = [dataSource parameterControlView:self floatForParameterAtIndexPath:indexPath];
		[controllerCell setFloatValue:val];
	}
	else if (fetchSelector == @selector(parameterControlView:doubleForParameterAtIndexPath:)) {
		double val = [dataSource parameterControlView:self doubleForParameterAtIndexPath:indexPath];
		[controllerCell setDoubleValue:val];
	}
    
}

-(void) reloadParameterValueForKey:(NSString*)key {
    
    NSAssert(dataSource, @"Need dataSource")
    
    if (!dataSource)
		return;
    
    NSIndexPath* indexPath = [dataSource parameterControlView:self indexPathForParameterWithKey:key];
    
    [self reloadParameterValueAtIndexPath:indexPath];
    
}

-(void) reloadAllParameterValues {
    
    NSInteger parameterCount = [dataSource parameterControlViewNumberOfParameters:self];
    
    for (NSInteger i = 0; i < parameterCount; i++) {
        NSIndexPath* path = [NSIndexPath indexPathWithIndex:i];
        [self reloadParameterValueAtIndexPath:path];
    }
    
}

#pragma mark - Reload parameter display strings

-(void) reloadParameterValueAtIndexPath:(NSIndexPath*)indexPath {
    
    if (!dataSource)
		return;
    
    NSInteger rowIndex = [indexPath indexAtPosition:0];
	
	NSAssert(rowIndex < parameterCount, @"Invalid row index");
	
	if (rowIndex >= parameterCount)
		rowIndex = parameterCount-1;
	
	NSString* displayString = [dataSource parameterControlView:self displayStringForParameterAtIndexPath:indexPath];
	NSCell* labelCell = [labelMatrix cellAtRow:rowIndex column:0];
	[labelCell setStringValue:displayString];
	
    
}

-(void) reloadParameterDisplayStringForKey:(NSString*)key {
    
    NSAssert(dataSource, @"Need dataSource")
    
    if (!dataSource)
		return;
    
    NSIndexPath* indexPath = [dataSource parameterControlView:self indexPathForParameterWithKey:key];
    
    [self reloadParameterDisplayStringAtIndexPath:indexPath];
    
}

-(void) reloadAllParameterDisplayStrings {
    
    NSInteger parameterCount = [dataSource parameterControlViewNumberOfParameters:self];
    
    for (NSInteger i = 0; i < parameterCount; i++) {
        NSIndexPath* path = [NSIndexPath indexPathWithIndex:i];
        [self reloadParameterDisplayStringAtIndexPath:path];
    }
    
}

#pragma mark Interface Helpers

-(NSSize) sizeOfString:(NSString*)s inCell:(NSCell*)cell {
	
	NSFont* font = [cell font];
	NSDictionary* attributes = [NSDictionary dictionaryWithObject:font forKey:NSFontAttributeName]];
	 
	return [s sizeWithAttributes:attributes];;
	
}


@end
