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
@class VSCOSXKeyed1FieldView;
@class VSCOSXKeyed3FieldView;
@class VSCOSXKeyed4FieldView;

@interface VSCOSXInterfaceFactory : NSObject

+(VSCOSXInterfaceFactory*) defaultFactory;

/*
 *  Generic
 */

@property (strong) NSNib* keyedCheckBoxViewNib;
@property (strong) NSNib* keyedSliderViewNib;
@property (strong) NSNib* keyed1FieldViewNib;
@property (strong) NSNib* keyed3FieldViewNib;
@property (strong) NSNib* keyed4FieldViewNib;

-(VSCOSXKeyedCheckBoxView*) newKeyedCheckBoxViewWithOwner:(id)owner;
-(VSCOSXKeyedSliderView*) newKeyedSliderViewWithOwner:(id)owner;
-(VSCOSXKeyed1FieldView*) newKeyed1FieldViewWithOwner:(id)owner;
-(VSCOSXKeyed3FieldView*) newKeyed3FieldViewWithOwner:(id)owner;
-(VSCOSXKeyed4FieldView*) newKeyed4FieldViewWithOwner:(id)owner;

/*
 *  Collision Action Views
 */

@property (strong) NSNib* keyedActionCommonViewNib;
@property (strong) NSNib* keyedActionMIDIOutputViewNib;
@property (strong) NSNib* keyedActionMIDIChannelViewNib;
@property (strong) NSNib* keyedActionMIDIControlNumberViewNib;

-(NSView*) newActionCommonViewWithOwner:(id)owner;
-(NSView*) newActionMIDIOutputViewWithOwner:(id)owner;
-(NSView*) newActionMIDIChannelViewWithOwner:(id)owner;
-(NSView*) newActionMIDIControlNumberViewWithOwner:(id)owner;

/*
 *  Table Views
 */

@property (strong) NSNib* tableSectionViewNib;

-(NSView*) newTableSectionViewWithOwner:(id)owner;

@end
