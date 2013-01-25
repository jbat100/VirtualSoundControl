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
@class VSCOSXKeyed3FieldView;
@class VSCOSXKeyed4FieldView;

@interface VSCOSXInterfaceFactory : NSObject

+(VSCOSXInterfaceFactory*) defaultFactory;

@property (strong) NSNib* keyedCheckBoxViewNib;
@property (strong) NSNib* keyedSliderViewNib;
@property (strong) NSNib* keyed3FieldViewNib;
@property (strong) NSNib* keyed4FieldViewNib;

-(VSCOSXKeyedCheckBoxView*) newKeyedCheckBoxViewWithOwner:(id)owner;
-(VSCOSXKeyedSliderView*) newKeyedSliderViewWithOwner:(id)owner;
-(VSCOSXKeyed3FieldView*) newKeyed3FieldViewWithOwner:(id)owner;
-(VSCOSXKeyed4FieldView*) newKeyed4FieldViewWithOwner:(id)owner;

@end
