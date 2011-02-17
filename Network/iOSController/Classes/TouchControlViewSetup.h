//
//  TouchControlViewSetup.h
//  iOSController
//
//  Created by Jonathan Thorpe on 13/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Foundation/Foundation.h>
#import "ControlViewSetup.h"

@protocol TouchControlViewSetup 

@property (nonatomic, assign) NSInteger xChannel;
@property (nonatomic, assign) NSInteger yChannel;

@end


@interface TouchControlViewSetup : ControlViewSetup <TouchControlViewSetup> {
	
	NSInteger xChannel;
	NSInteger yChannel;

}

@property (nonatomic, assign) NSInteger xChannel;
@property (nonatomic, assign) NSInteger yChannel;

@end
