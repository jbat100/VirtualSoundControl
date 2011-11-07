//
//  VSCParameterControlView.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCSound.h"


@protocol VSCParameterControlViewDelegate

@required

-(void) parameterControlView:(VSCParameterControlView*)view changedParameterWithKey:(NSString*)key;
-(NSString*) parameterControlView:(VSCParameterControlView*)view displayStringForParameterWithKey:(NSString*)key;
-(NSInteger) parameterControlView:(VSCParameterControlView*)view displayIndexForParameterWithKey:(NSString*)key;
-(SEL) parameterControlView:(VSCParameterControlView*)view fetchSelectorForParameterWithKey:(NSString*)key;

@optional

-(NSString*) parameterControlView:(VSCParameterControlView*)view stringForParameterWithKey:(NSString*)key;
-(float) parameterControlView:(VSCParameterControlView*)view floatForParameterWithKey:(NSString*)key;
-(double) parameterControlView:(VSCParameterControlView*)view doubleForParameterWithKey:(NSString*)key;

@end



@interface VSCParameterControlView : NSView {
	
	NSMutableSet* parameterKeys;
	
	NSScrollView* scrollView;

	/*
	 *	Keep two different matrices for labels and sliders (so that they can have 
	 *	different sizes).
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
}

@property (nonatomic, retain) NSScrollView* scrollView; 

@property (nonatomic, retain) NSMatrix* controllerMatrix;
@property (nonatomic, retain) NSMatrix* labelMatrix;

@property (nonatomic, retain) NSActionCell* controllerCellPrototype;
@property (nonatomic, retain) NSCell* labelCellPrototype;

@property (nonatomic) CGFloat centerSpacing;

@property (assign) id<VSCParameterControlViewDelegate> delegate;


-(void) addParameterWithKey:(NSString*)key;
-(void) removeParameterWithKey:(NSString*)key;
-(void) removeAllParameters;

-(NSSet*) parameterKeys;

-(void) reloadParameterForKey:(NSString*)key;
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
