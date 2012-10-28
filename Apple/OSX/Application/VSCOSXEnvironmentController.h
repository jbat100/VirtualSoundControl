//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCEnvironment.h"
#include "VSCOBScene.h"

@protocol VSCOSXEnvironmentController <NSObject>

@property (nonatomic, assign) VSC::Environment::WPtr environment;

-(void) showEnvironmentInspector;

-(void) showElementInspectorForElement:(VSC::OB::Scene::Element::SPtr)element;

@end
