//
//  NSFileManager-MenuItems.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 27/09/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface NSFileManager (MenuItem)

/*
 *	Creates a menu item (including submenu items for all descendant file tree structure)
 */
-(NSMenuItem*) menuItemAtPath:(NSString*)path;

@end
