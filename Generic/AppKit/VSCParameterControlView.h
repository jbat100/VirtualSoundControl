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
-(NSString*) displayStringForParameterWithKey:(NSString*)key;

@optional

-(NSString*) stringForParameterWithKey:(NSString*)key;
-(float) floatForParameterWithKey:(NSString*)key;
-(double) doubleForParameterWithKey:(NSString*)key;
-(VSCSFloat) vscsFloatForParameterWithKey:(NSString*)key;

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
}

@property (nonatomic, retain) NSScrollView* scrollView; 

@property (nonatomic, retain) NSMatrix* controllerMatrix;
@property (nonatomic, retain) NSMatrix* labelMatrix;

@property (nonatomic, retain) NSActionCell* controllerCellPrototype;
@property (nonatomic, retain) NSCell* labelCellPrototype;


-(void) addParameterWithKey:(NSString*)key;
-(void) removeParameterWithKey:(NSString*)key;
-(NSSet*) parameterKeys;

-(void) updateInterface;
-(void) resetInterface; // recreates the matrix cells with the current prototype


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
