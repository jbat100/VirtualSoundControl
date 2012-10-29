//
//  VSCIMOSXCollisionActionChainView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/29/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "PXListView.h"
#import "VSCIMOSXCollisionActionView.h"

@interface VSCIMOSXCollisionActionChainView : NSView <PXListViewDelegate, VSCIMOSXCollisionActionViewDelegate>

@property (nonatomic, retain) PXListView* collisionActionListView;

/*
 *  Collision Actions
 */

-(void) prependNewCollisionAction;
-(void) appendNewCollisionAction;
-(void) insertNewCollisionActionBeforeSelection;
-(void) insertNewCollisionActionAfterSelection;

/*
 *  Delays
 */

-(void) prependNewDelay;
-(void) appendNewDelay;
-(void) insertNewDelayBeforeSelection;
-(void) insertNewDelayAfterSelection;

@end
