//
//  VSCParameterControlView.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCSound.h"

@class VSCParameterControlView;


@protocol VSCParameterControlViewDelegate

@optional

-(void) parameterControlView:(VSCParameterControlView*)view changedParameterWithKey:(NSString*)key;

@end



@protocol VSCParameterControlViewDataSource

@required

-(NSInteger) parameterControlViewNumberOfParameters:(VSCParameterControlView*)view;
-(NSIndexPath*) parameterControlView:(VSCParameterControlView*)view indexPathForParameterWithKey:(NSString*)key;

-(NSString*) parameterControlView:(VSCParameterControlView*)view parameterKeyForParameterAtIndexPath:(NSIndexPath*)indexPath;
-(NSString*) parameterControlView:(VSCParameterControlView*)view displayStringForParameterAtIndexPath:(NSIndexPath*)indexPath;

-(SEL) parameterControlView:(VSCParameterControlView*)view fetchSelectorForParameterAtIndexPath:(NSIndexPath*)indexPath;

@optional

-(NSString*) parameterControlView:(VSCParameterControlView*)view stringForParameterAtIndexPath:(NSIndexPath*)indexPath;
-(float) parameterControlView:(VSCParameterControlView*)view floatForParameterAtIndexPath:(NSIndexPath*)indexPath;
-(double) parameterControlView:(VSCParameterControlView*)view doubleForParameterAtIndexPath:(NSIndexPath*)indexPath;

@end



@interface VSCParameterControlView : NSView {
	
	NSScrollView* scrollView;

	/*
	 *	Keep two different matrices for labels and sliders (so that they can have 
	 *	different sizes). USE sizeToCells TO RESIZE !!!
	 */
	
	NSMatrix * controllerMatrix;
	NSMatrix * labelMatrix;
	
	/*
	 *	Prototype NSCells fo the controllers and labels
	 */
	
	NSActionCell* controllerCellPrototype;
	NSCell* labelCellPrototype;
	
	/*
	 *	Interface setup parameters 
	 */
	CGFloat centerSpacing;
	
	id<VSCParameterControlViewDelegate> delegate;
	id<VSCParameterControlViewDataSource> dataSource;
}

@property (nonatomic, retain) NSScrollView* scrollView; 

@property (nonatomic, retain) NSMatrix* controllerMatrix;
@property (nonatomic, retain) NSMatrix* labelMatrix;

@property (nonatomic, retain) NSActionCell* controllerCellPrototype;
@property (nonatomic, retain) NSCell* labelCellPrototype;

@property (nonatomic) CGFloat centerSpacing;

@property (assign) id<VSCParameterControlViewDelegate> delegate;
@property (assign) id<VSCParameterControlViewDataSource> dataSource;


-(void) reloadParameterValueForKey:(NSString*)key;
-(void) reloadParameterValueAtIndexPath:(NSIndexPath*)indexPath;
-(void) reloadAllParameterValues;

-(void) reloadParameterDisplayStringForKey:(NSString*)key;
-(void) reloadParameterDisplayStringAtIndexPath:(NSIndexPath*)indexPath;
-(void) reloadAllParameterDisplayStrings;

-(void) updateInterface;
-(void) resetInterface; // recreates the matrix cells with the current prototype

-(NSSize) sizeOfString:(NSString*)s inCell:(NSCell*)cell;


/*
 *	Private stuff which I can't be bothered putting in a separate header file...
 */

-(void) customInit;
// matrix creation
-(void) createControllerMatrix;
-(void) createLabelMatrix;
// matrix destruction
-(void) createMatrices;
-(void) destroyMatrices;

@end
