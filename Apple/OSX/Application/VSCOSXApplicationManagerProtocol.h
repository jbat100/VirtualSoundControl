//
//  VSCOSXApplicationManagerProtocol.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/13/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>

#include "VSCGlobalApplication.h"


@protocol VSCOSXApplicationManagerProtocol <NSObject>

@property(nonatomic, assign) VSC::GlobalApplication::SPtr application;

-(void) startOgreRendering;
-(void) stopOgreRendering;

@end
