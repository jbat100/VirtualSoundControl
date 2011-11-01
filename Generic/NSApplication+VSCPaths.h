//
//  NSApplication+VSCPaths.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 24/10/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>


@interface NSApplication (VSCPaths) 

- (NSString *) applicationSupportDirectory;
- (NSString *) applicationLibraryDirectory;

@end
