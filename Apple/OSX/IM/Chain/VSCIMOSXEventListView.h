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
@property (weak) IBOutlet NSPopUpButton* appendEventButton;
@property (weak) IBOutlet NSPopUpButton* insertEventButton;
@property (weak) IBOutlet NSButton* removeEventButton;
@property (weak) IBOutlet NSMenu* addEventMenu;

@property (weak) IBOutlet NSMenuItem* appendDelayMenuItem;
@property (weak) IBOutlet NSMenuItem* appendActionMenuItem;
@property (weak) IBOutlet NSMenuItem* insertDelayMenuItem;
@property (weak) IBOutlet NSMenuItem* insertActionMenuItem;

@end
