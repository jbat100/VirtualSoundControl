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
 *	Interface creation/destruction
 */

-(void) customInit;
-(void) createMatrices;
-(void) destroyMatrices;

@end
