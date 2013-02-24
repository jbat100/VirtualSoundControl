//
//  VSCOBOSXElementEventChainView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

/*
 *  A view for displaying collision related element stuff, it can show multiple
 *  collision edit views for different stages of the collision.
 */

@protocol VSCOBOSXElementController;

@interface VSCOBOSXElementEventChainsView : NSView

@property (assign) IBOutlet id<VSCOBOSXElementController> elementController;

@property (nonatomic, strong) IBOutlet NSTableView* eventChainTableView;

@end
