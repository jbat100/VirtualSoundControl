//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCEnvironment.h"
#include "VSCEnvironmentTest.h"
#include "VSCOB.h"
#include "VSCIM.h"


@protocol VSCOBOSXSceneController;

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
@property (strong) IBOutlet id<VSCOBOSXSceneController> sceneController;

-(void) addEventChain;
-(void) removeEventChain:(VSC::IM::EventChain_SPtr)eventChain;

-(void) showEnvironmentInspector;
-(void) showSceneDetails;
-(void) showEventChainList;
-(void) showElementList;
-(void) showElementInspectorForElement:(VSC::OB::Element_SPtr)element;
-(void) showEventChainEditor:(VSC::IM::EventChain_SPtr)eventChain;

@end
