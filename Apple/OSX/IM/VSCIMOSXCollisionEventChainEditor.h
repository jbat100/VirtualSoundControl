
#import <Foundation/Foundation.h>

/*
 *  VSCIMOSXCollisionEventChainEditor, meant for the VSCIMOSXCollisionEventChainView
 */

@protocol VSCIMOSXCollisionEventChainEditor <NSObject>

-(void) sender:(id)sender requestsMappingEditorForAction:(VSC::IM::CollisionAction::SPtr)action;
-(void) senderRequestsEventChainView:(id)sender;

@end

