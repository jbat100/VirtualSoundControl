//
//  VSCIMActionView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "VSCIMOSXEventCellView.h"

#include "VSCIM.h"

/*
 *  The actual NSView subclass which displays the collision action parameters and allows 
 *  the type of action to be switched.
 */

@interface VSCIMOSXActionCellView : VSCIMOSXEventCellView

+(CGFloat) defaultViewHeight;

-(VSC::IM::Action_SPtr) action; // return cast event from VSCIMOSXEventCellView

@property (weak) IBOutlet NSPopUpButton* actionTypePopUpButton;

-(IBAction)actionTypeSelected:(id)sender;

@end
