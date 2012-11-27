#import <Foundation/Foundation.h>

/*
 *  VSCIMOSXCollisionEventChainController, meant for the VSCIMOSXCollisionEventChainView
 */

#import "PXListViewDelegate.h"

@protocol VSCOSXOBSceneElementController;

@protocol VSCIMOSXCollisionEventChainController <NSObject, PXListViewDelegate>

@property (weak) id<VSCOSXOBSceneElementController> elementController;

-(void) sender:(id)sender requestsShowMappingsForCollisionAction:(VSC::IM::CollisionAction::SPtr)action;
-(void) senderRequestsEventCollisionChainView:(id)sender;

@end