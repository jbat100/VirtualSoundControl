
#import <Foundation/Foundation.h>

#include "VSCIM.h"

/*
 *  VSCIMOSXEventChainController, meant for the VSCIMOSXEventChainView
 */

@protocol VSCOSXEnvironmentController;

@protocol VSCIMOSXEventChainController <NSObject>

@property (assign) id<VSCOSXEnvironmentController> environmentController;

@property (nonatomic, assign) VSC::IM::EventChain_WPtr eventChain;

-(void) sender:(id)sender requestsShowEventEditorForEvent:(VSC::IM::Event_SPtr)action;
-(void) senderRequestsEventChainView:(id)sender;

@end