//
//  VSCIMOSXCollisionEventChainController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/29/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "PXListViewDelegate.h"

#include "VSCIMCollisionEventChain.h"
#include "VSCIMEvent.h"

/*
 *  Using a protocol to decrease coupling
 */

@protocol VSCIMOSXCollisionEventChainController <NSObject, PXListViewDelegate>

@property (nonatomic, assign) VSC::IM::CollisionEventChain::WPtr collisionEventChain;

@end

/*
 *  A concrete VSCIMOSXCollisionEventChainController
 */

@class VSCIMOSXCollisionEventChainView;
@class VSCIMOSXCollisionMappingView;

@interface VSCIMOSXCollisionEventChainController : NSObject <VSCIMOSXCollisionEventChainController>

@property (nonatomic, strong) IBOutlet VSCIMOSXCollisionEventChainView* collisionEventChainView;
@property (nonatomic, strong) NSNib* mappingViewNib;

/*
 *  Action view callbacks
 */

-(IBAction) showCollisionMappings:(id)sender;
-(IBAction) refreshMIDIOutputs:(id)sender;
-(IBAction) midiOutputSelected:(id)sender;
-(IBAction) refreshMIDIControlNumbers:(id)sender;
-(IBAction) midiControlNumberSelected:(id)sender;

/*
 *  Add/Remove chain events
 */

-(IBAction) removeSelectedEvent:(id)sender;

-(IBAction) prependNewCollisionAction:(id)sender;
-(IBAction) appendNewCollisionAction:(id)sender;
-(IBAction) insertNewCollisionActionBeforeSelection:(id)sender;
-(IBAction) insertNewCollisionActionAfterSelection:(id)sender;

-(IBAction) prependNewDelay:(id)sender;
-(IBAction) appendNewDelay:(id)sender;
-(IBAction) insertNewDelayBeforeSelection:(id)sender;
-(IBAction) insertNewDelayAfterSelection:(id)sender;

/*
 *  Factory methods
 */

-(VSCIMOSXCollisionMappingView*) newCollisionMappingView;


@end
