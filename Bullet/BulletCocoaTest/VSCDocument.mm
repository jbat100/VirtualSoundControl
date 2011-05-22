//
//  VSCDocument.m
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 21/04/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "VSCDocument.h"
#import "VSCBaseApplication.h"


@implementation VSCDocument

@synthesize vscOpenGLView;

-(id) init {
	
	if ((self = [super init])) {
		baseApplication = new VSCBaseApplication();
		baseApplication->initPhysics();
		//baseApplication->getDynamicsWorld()->setDebugDrawer(&sDebugDraw);
	}
	
	return self;
	
}

-(void) dealloc {
	
	vscOpenGLView.delegate = nil;
	self.vscOpenGLView = nil;
	delete baseApplication;
	[super dealloc];
}

- (NSString *)windowNibName {
    // Implement this to return a nib to load OR implement -makeWindowControllers to manually create your controllers.
    return @"VSCDocument";
}

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

-(VSCBaseApplication*) baseApplication {
	return baseApplication;
}

@end
