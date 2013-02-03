

#import <Foundation/Foundation.h>

#include "VSCIMAction.h"

/*
 *  VSCOBOSXElementEditor, meant for the main inspector view
 */

@protocol VSCOBOSXElementEditor <NSObject>

-(void) senderRequestsDetailView:(id)sender;
-(void) senderRequestsCollisionView:(id)sender;

@end


/*
 *  VSCIMOSXActionEditor
 */

@protocol VSCIMOSXActionEditor <NSObject>

@end
