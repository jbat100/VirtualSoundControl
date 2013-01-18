//
//  VSCOSXInterfaceFactory.h
//  VSCApplication
//
//  Created by Jonathan Thorpe on 1/18/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class VSCOSXKeyedCheckBoxView;
@class VSCOSXKeyedSliderView;

@interface VSCOSXInterfaceFactory : NSObject

+(VSCOSXInterfaceFactory*) defaultFactory;

@property (strong) NSNib* keyedCheckBoxViewNib;
@property (strong) NSNib* keyedSliderViewNib;

-(VSCOSXKeyedCheckBoxView*) newKeyedCheckBoxViewWithOwner:(id)owner;
-(VSCOSXKeyedCheckBoxView*) newKeyedSliderViewWithOwner:(id)owner;

@end
