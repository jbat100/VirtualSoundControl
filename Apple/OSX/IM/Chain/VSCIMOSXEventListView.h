//
//  VSCIMOSXEventListView.h
//  VSCApplication
//
//  Created by Jonathan Thorpe on 2/17/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@protocol VSCIMOSXEventChainController;

@interface VSCIMOSXEventListView : NSView

@property (weak) id<VSCIMOSXEventChainController> eventChainController;

/*
 *  Event chain interface
 */

@property (weak) IBOutlet NSTableView* eventTableView;
@property (weak) IBOutlet NSPopUpButton* addEventButton;
@property (weak) IBOutlet NSButton* removeEventButton;
@property (weak) IBOutlet NSMenu* addEventMenu;

@property (weak) IBOutlet NSMenuItem* addDelayMenuItem;
@property (weak) IBOutlet NSMenuItem* addActionMenuItem;

@end
