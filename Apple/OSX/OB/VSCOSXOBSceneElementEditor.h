

#import <Foundation/Foundation.h>

#include "VSCIMAction.h"

/*
 *  VSCOSXOBSceneElementEditor, meant for the main inspector view
 */

@protocol VSCOSXOBSceneElementEditor <NSObject>

-(void) senderRequestsDetailView:(id)sender;
-(void) senderRequestsCollisionView:(id)sender;

@end


/*
 *  VSCIMOSXActionEditor
 */

@protocol VSCIMOSXActionEditor <NSObject>

@end
