//
//  NSFileManager-MenuItems.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 27/09/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "NSFileManager-MenuItem.h"


@implementation NSFileManager (MenuItem)

-(NSMenuItem*) menuItemAtPath:(NSString*)path {
	
	NSFileManager* fileManager = [NSFileManager defaultManager];
	
	BOOL isDirectory = NO;
	BOOL exists = [fileManager fileExistsAtPath:path isDirectory:&isDirectory];
	
	if (!exists)
		return nil;
	
	NSMenuItem* item = nil;
	
	if (isDirectory) {
		
		item = [[NSMenu alloc] initWithTitle:[path lastPathComponent] action:NULL keyEquivalent:@""];
		
		NSMenu* subMenu = [[NSMenu alloc] initWithTitle:[path lastPathComponent]];
		
		NSArray *children = [fileManager contentsOfDirectoryAtPath:path error:nil];
		for (NSString* child in children) {
			[subMenu addItem:[self menuItemAtPath:[path stringByAppendingPathComponent:child]]];
		}
		
		if ([[subMenu itemArray] count] > 0) {
			[item setSubmenu:subMenu];
		}
	}
	
	else {
		item = [[NSMenuItem alloc] initWithTitle:path action:NULL keyEquivalent:@""];
	}
	
	return [item autorelease];
}

@end
