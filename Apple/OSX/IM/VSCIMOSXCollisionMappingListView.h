//
//  VSCIMOSXCollisionMappingListView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/8/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class PXListView;

/*
 *  Container view for mapping views.
 */

@interface VSCIMOSXCollisionMappingListView : NSView

@property (weak) IBOutlet PXListView* listView;

@end
