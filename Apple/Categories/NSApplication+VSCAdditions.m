//
//  NSApplication+VSCPaths.m
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 24/10/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "NSApplication+VSCAdditions.h"


@implementation NSApplication (VSCPaths)

- (NSString *) applicationSupportDirectory {	
	NSArray* paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
	NSString *dir = [paths lastObject];
	if (![[NSFileManager defaultManager] fileExistsAtPath:dir])
		[[NSFileManager defaultManager] createDirectoryAtPath:dir withIntermediateDirectories:YES attributes:nil error:nil];
	return dir;
}

- (NSString *) applicationLibraryDirectory {	
	NSArray* paths = NSSearchPathForDirectoriesInDomains(NSLibraryDirectory, NSUserDomainMask, YES);
	NSString *dir = [paths lastObject];
	if (![[NSFileManager defaultManager] fileExistsAtPath:dir])
		[[NSFileManager defaultManager] createDirectoryAtPath:dir 
								  withIntermediateDirectories:YES attributes:nil error:nil];
	return dir;
}

@end
