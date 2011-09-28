//
//  EnveloppeDocument.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 10/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "VSCEnveloppeEditorDocument.h"

@implementation VSCEnveloppeEditorDocument

static NSArray* enveloppeBaseFilePaths = nil;

-(id) init {
	
	if ((self = [super init])) {
		[self createDefaultEnveloppe];
	}
	
	return self;
	
}

#pragma mark - NSDocument Methods

/*
- (NSString *)windowNibName {
    // Implement this to return a nib to load OR implement -makeWindowControllers to manually create your controllers.
    return @"VSCEnveloppeDocument";
}
 */

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

-(void) makeWindowControllers {
	
	NSLog(@"%@ windowControllers are %@", self, [self windowControllers]);
	
	NSLog(@"%@ makeWindowControllers", self);
	
	enveloppeEditorWindowController = [[VSCEnveloppeEditorWindowController alloc] 
									   initWithWindowNibName:@"VSCEnveloppeEditorWindowController"];
	
	[self addWindowController:enveloppeEditorWindowController];
	
	NSLog(@"%@ added window controller %@", self, enveloppeEditorWindowController);
	
	[enveloppeEditorWindowController release];
	
	[enveloppeEditorWindowController.loadedTextField setStringValue:@"LOADED WITHOUT CALLBACKS"];
	
	NSLog(@"%@ windowControllers are %@", self, [self windowControllers]);
	
}

- (void)windowControllerWillLoadNib:(NSWindowController *)windowController {
	NSLog(@"%@ window %@ windowControllerWillLoadNib", self, windowController);
	if (windowController == enveloppeEditorWindowController) {
		[enveloppeEditorWindowController.loadedTextField setStringValue:@"LOADED WILL LOAD FROM DOCUMENT"];
	}
}


- (void)windowControllerDidLoadNib:(NSWindowController *)windowController {
	NSLog(@"%@ window %@ windowControllerDidLoadNib", self, windowController);
	if (windowController == enveloppeEditorWindowController) {
		[enveloppeEditorWindowController.loadedTextField setStringValue:@"LOADED DID LOAD FROM DOCUMENT"];
	}
}

#pragma mark - Enveloppe Base File Paths

- (NSString *) applicationSupportDirectory {	
	NSArray* paths = NSSearchPathForDirectoriesInDomains(NSApplicationSupportDirectory, NSUserDomainMask, YES);
	NSString *dir = [paths lastObject];
	
	if (![[NSFileManager defaultManager] fileExistsAtPath:dir])
		[[NSFileManager defaultManager] createDirectoryAtPath:dir withIntermediateDirectories:YES attributes:nil error:nil];
	
	return dir;
}

-(NSArray*) enveloppeBaseFilePaths {
	
	if (!enveloppeBaseFilePaths) {
		@synchronized(self) {
			NSString* defaultEnveloppeFilePath = [[self applicationSupportDirectory] 
												  stringByAppendingPathComponent:@"Enveloppes"];
			enveloppeBaseFilePaths = [NSArray arrayWithObjects:defaultEnveloppeFilePath, nil];
		}
	}
	
	return enveloppeBaseFilePaths;
}

#pragma mark - Enveloppe Access/Creation/Add/Remove

-(void) createDefaultEnveloppe {
	
	VSCSTKEnveloppePtr enveloppe = VSCSTKEnveloppePtr(new VSCSTKEnveloppe());
	
	enveloppe->addPoint(VSCEnveloppePointPtr(new VSCEnveloppePoint(0.0, 0.0)));
	enveloppe->addPoint(VSCEnveloppePointPtr(new VSCEnveloppePoint(0.0, 1.0)));
	enveloppe->addPoint(VSCEnveloppePointPtr(new VSCEnveloppePoint(0.0, 2.0)));
	enveloppe->addPoint(VSCEnveloppePointPtr(new VSCEnveloppePoint(0.0, 3.0)));
	
	enveloppe->setRelativePath(std::string([DEFAULT_ENVELOPPE_RELATIVE_PATH cString]));
	
	[self addEnveloppe:enveloppe];
	
}

-(void) addEnveloppe:(VSCEnveloppePtr)enveloppe {
	_mainEnveloppeList.push_back(enveloppe);
}

-(void) removeEnveloppe:(VSCEnveloppePtr)enveloppe {
	_mainEnveloppeList.remove(enveloppe);
}

-(ConstEnvIter) beginMainEnveloppeListIterator {
	return _mainEnveloppeList.begin();
}

-(ConstEnvIter) endMainEnveloppeListIterator {
	return _mainEnveloppeList.end();
}

-(VSCEnveloppePtr) defaultEnveloppe {
	std::string defaultRelativePath = std::string([DEFAULT_ENVELOPPE_RELATIVE_PATH cString]);
	for (EnvIter envIt = [self beginMainEnveloppeListIterator]; envIt != [self endMainEnveloppeListIterator]; envIt++) {
		VSCEnveloppe* enveloppe = (*envIt)->get();
		std::string relativePath = enveloppe->getRelativePath();
		if (relativePath.compare(defaultRelativePath) == 0) {
			return *envIt;
		}
	}
	return VSCEnveloppePtr();
}

@end
