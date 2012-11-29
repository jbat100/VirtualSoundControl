
#import <Foundation/Foundation.h>

#include "VSCIMCollisionAction.h"
#include "VSCIMCollisionEventChain.h"

/*
 *  VSCIMOSXCollisionEventChainController, meant for the VSCIMOSXCollisionEventChainView
 */

@protocol VSCOSXOBSceneElementController;

@protocol VSCIMOSXCollisionEventChainController <NSObject>

@property (assign) id<VSCOSXOBSceneElementController> elementController;
@property (nonatomic, assign) VSC::IM::CollisionEventChain::WPtr eventChain;

-(void) sender:(id)sender requestsShowMappingsForCollisionAction:(VSC::IM::CollisionAction::SPtr)action;
-(void) senderRequestsEventCollisionChainView:(id)sender;

@end