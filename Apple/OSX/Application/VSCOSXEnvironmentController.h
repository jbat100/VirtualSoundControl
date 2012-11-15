//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCEnvironment.h"
#include "VSCOBScene.h"


@protocol VSCOSXOBSceneController;


/*
 *  Main environment controller protocol
 */

@protocol VSCOSXEnvironmentController <NSObject>

/*
 *  The actual environment the controller is associated with
 */
@property (nonatomic, assign) VSC::Environment::WPtr environment;
/*
 *  Sub-controllers
 */
@property (strong) IBOutlet id<VSCOSXOBSceneController> sceneController;

-(void) showEnvironmentInspector;

-(void) showSceneSettings;
-(void) showSceneElementList;

-(void) showElementInspectorForElement:(VSC::OB::Scene::Element::SPtr)element;

@end
