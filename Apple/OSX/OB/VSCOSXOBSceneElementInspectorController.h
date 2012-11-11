//
//  VSCOSXOBSceneElementInspectorController.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/10/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

@protocol VSCIMOSXCollisionEventChainController;
@class VSCOSXOBSceneElementInspectorView;


@protocol VSCOSXOBSceneElementInspectorController <NSObject>

@property (weak) id<VSCIMOSXCollisionEventChainController> collisionEventChainController;

@property (weak) IBOutlet VSCOSXOBSceneElementInspectorView* elementInspectorView;

@end
