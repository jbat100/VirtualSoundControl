//
//  VSCParameterControlView.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 06/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCParameterControlViewProtocol.h"

#import "VSCSound.h"
#import "VSCSoundParameters.h"


@interface VSCParameterControlView : NSView <VSCParameterControlViewProtocol> {
	
	
	
	VSCParameterControlOptions parameterControlOptions;
	NSInteger numberOfParameters;
	
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
	CGFloat centerSpacing;
	
	id<VSCParameterControlViewDelegate> delegate;
}

@property (nonatomic, assign) VSCParameterControlOptions parameterControlOptions;
@property (nonatomic, assign) NSInteger numberOfParameters;

@property (nonatomic, retain) NSScrollView* scrollView; 

@property (nonatomic, retain) NSMatrix* controllerMatrix;
@property (nonatomic, retain) NSMatrix* labelMatrix;
@property (nonatomic, retain) NSMatrix* numericMatrix;

@property (nonatomic, retain) NSActionCell* controllerCellPrototype;
@property (nonatomic, retain) NSCell* labelCellPrototype;
@property (nonatomic, retain) NSCell* numericCellPrototype;

@property (nonatomic, assign) CGFloat centerSpacing;

@property (assign) id<VSCParameterControlViewDelegate> delegate;



@end
