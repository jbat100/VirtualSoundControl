//
//  VSCParameterControlView_Private.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 13/11/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface VSCParameterControlView (Private)

/*
 *	NSCell access and utility
 */

-(NSCell*) controllerCellAtIndex:(NSInteger)index;
-(NSCell*) labelCellAtIndex:(NSInteger)index;
-(NSCell*) numericCellAtIndex:(NSInteger)index;

-(NSSize) sizeOfString:(NSString*)s inCell:(NSCell*)cell;

/*
 *	Interface creation/destruction
 */

-(void) customInit;
// matrix creation
-(void) createControllerMatrix;
-(void) createLabelMatrix;
-(void) createNumericMatrix;
// matrix destruction
-(void) createMatrices;
-(void) destroyMatrices;



@end
