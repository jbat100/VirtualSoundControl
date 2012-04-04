//
//  SynthStationDocument.m
//  SynthStation
//
//  Created by Jonathan Thorpe on 24/10/2011.
//  Copyright 2011 NXP. All rights reserved.
//

#import "VSCSynthStationDocument.h"
#import "NSApplication+VSCAdditions.h"
#import "VSCSynthStationWindowController.h"

@implementation VSCSynthStationDocument

#pragma mark Data

- (NSData *)dataOfType:(NSString *)typeName error:(NSError **)outError
{
    // Insert code here to write your document to data of the specified type. If the given outError != NULL, ensure that you set *outError when returning nil.

    // You can also choose to override -fileWrapperOfType:error:, -writeToURL:ofType:error:, or -writeToURL:ofType:forSaveOperation:originalContentsURL:error: instead.

    // For applications targeted for Panther or earlier systems, you should use the deprecated API -dataRepresentationOfType:. In this case you can also choose to override -fileWrapperRepresentationOfType: or -writeToFile:ofType: instead.

    return nil;
}

- (BOOL)readFromData:(NSData *)data ofType:(NSString *)typeName error:(NSError **)outError
{
    // Insert code here to read your document from the given data of the specified type.  If the given outError != NULL, ensure that you set *outError when returning NO.

    // You can also choose to override -readFromFileWrapper:ofType:error: or -readFromURL:ofType:error: instead. 
    
    // For applications targeted for Panther or earlier systems, you should use the deprecated API -loadDataRepresentation:ofType. In this case you can also choose to override -readFromFile:ofType: or -loadFileWrapperRepresentation:ofType: instead.
    
    return YES;
}

#pragma mark Window Controllers

-(void) makeWindowControllers {
	
	NSLog(@"%@ windowControllers are %@", self, [self windowControllers]);
	
	NSLog(@"%@ makeWindowControllers", self);
	
	synthStationWindowController = [[VSCSynthStationWindowController alloc] 
									   initWithWindowNibName:@"VSCSynthStationWindowController"];
	
	[self addWindowController:synthStationWindowController];
	
	NSLog(@"%@ added window controller %@", self, synthStationWindowController);
	
	[synthStationWindowController release];
	
	NSLog(@"%@ windowControllers are %@", self, [self windowControllers]);
	
}

- (void)windowControllerWillLoadNib:(NSWindowController *)windowController {
	NSLog(@"%@ window %@ windowControllerWillLoadNib", self, windowController);
}


- (void)windowControllerDidLoadNib:(NSWindowController *)windowController {
	NSLog(@"%@ window %@ windowControllerDidLoadNib", self, windowController);
}

@end
