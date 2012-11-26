

#import <Foundation/Foundation.h>

#include "VSCIMCollisionAction.h"

/*
 *  VSCOSXOBSceneElementEditor, meant for the main inspector view
 */

@protocol VSCOSXOBSceneElementEditor <NSObject>

-(void) senderRequestsDetailView:(id)sender;
-(void) senderRequestsCollisionView:(id)sender;

@end


/*
 *  VSCIMOSXCollisionActionEditor
 */

@protocol VSCIMOSXCollisionActionEditor <NSObject>

@end
