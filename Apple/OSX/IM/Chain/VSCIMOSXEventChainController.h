
#import <Foundation/Foundation.h>

#include "VSCIMAction.h"
#include "VSCIMEventChain.h"

#include "VSCIMDelay.h"

/*
 *  VSCIMOSXEventChainController, meant for the VSCIMOSXEventChainView
 */

@protocol VSCOSXOBSceneElementController;

@protocol VSCIMOSXEventChainController <NSObject>

@property (assign) id<VSCOSXOBSceneElementController> elementController;
@property (nonatomic, assign) VSC::IM::EventChain::WPtr eventChain;

-(void) sender:(id)sender requestsShowMappingsForAction:(VSC::IM::Action::SPtr)action;
-(void) sender:(id)sender requestsSetDelay:(VSC::IM::Delay::SPtr)delay toInterval:(NSTimeInterval)delayInterval;
-(void) senderRequestsEventCollisionChainView:(id)sender;

@end