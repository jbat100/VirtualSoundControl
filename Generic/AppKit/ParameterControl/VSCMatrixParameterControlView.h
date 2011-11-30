//
//  VSCParameterControlView.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCParameterControlView.h"
#import "VSCSound.h"
#import "VSCSoundParameters.h"

#import <set>
#import <boost/bimap.hpp>


@interface VSCMatrixParameterControlView : VSCParameterControlView  {
	
	VSCSParameter::KeyIndexBimap parameterKeyIndexBimap;
	
	NSScrollView* scrollView;

	/*
	 *	Keep two different matrices for labels and sliders (so that they can have 
	 *	different sizes). USE sizeToCells TO RESIZE !!!
	 */
	NSMatrix* controllerMatrix;
	NSMatrix* labelMatrix;
	NSMatrix* numericMatrix;
	
	/*
	 *	Prototype NSCells fo the controllers and labels
	 */
	NSActionCell* controllerCellPrototype;
	NSCell* labelCellPrototype;
	NSCell* numericCellPrototype;
	
	/*
	 *	Interface setup parameters 
	 */
	CGFloat spacing;
	
}

@property (nonatomic, retain) NSScrollView* scrollView; 

@property (nonatomic, retain) NSMatrix* controllerMatrix;
@property (nonatomic, retain) NSMatrix* labelMatrix;
@property (nonatomic, retain) NSMatrix* numericMatrix;

@property (nonatomic, retain) NSActionCell* controllerCellPrototype;
@property (nonatomic, retain) NSCell* labelCellPrototype;
@property (nonatomic, retain) NSCell* numericCellPrototype;

@property (nonatomic, assign) CGFloat spacing;

-(void) setparameterKeyIndexBimap:(VSCSParameter::KeyIndexBimap)keyIndexBymap;
-(const VSCSParameter::KeyIndexBimap&) parameterKeyIndexBimap;

-(VSCSParameter::Key) keyForParameterAtIndex:(NSInteger)index;
-(NSInteger) indexForParameterWithKey:(VSCSParameter::Key)key;

-(VSCSParameter::Key) parameterKeyForCell:(NSCell*)cell;

-(NSActionCell*) controllerCellForParameterWithKey:(VSCSParameter::Key k);
-(NSCell*) numericCellForParameterWithKey:(VSCSParameter::Key k);
-(NSCell*) labelCellForParameterWithKey:(VSCSParameter::Key k);

@end
