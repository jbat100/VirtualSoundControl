
#import <Foundation/Foundation.h>

#include "VSCIMAction.h"
#include "VSCIMCollisionEventChain.h"

#include "VSCIMDelay.h"

/*
 *  VSCIMOSXCollisionEventChainController, meant for the VSCIMOSXCollisionEventChainView
 */

@protocol VSCOSXOBSceneElementController;

@protocol VSCIMOSXCollisionEventChainController <NSObject>

@property (assign) id<VSCOSXOBSceneElementController> elementController;
@property (nonatomic, assign) VSC::IM::CollisionEventChain::WPtr eventChain;

-(void) sender:(id)sender requestsShowMappingsForCollisionAction:(VSC::IM::CollisionAction::SPtr)action;
-(void) sender:(id)sender requestsSetDelay:(VSC::IM::Delay::SPtr)delay toInterval:(NSTimeInterval)delayInterval;
-(void) senderRequestsEventCollisionChainView:(id)sender;

@end