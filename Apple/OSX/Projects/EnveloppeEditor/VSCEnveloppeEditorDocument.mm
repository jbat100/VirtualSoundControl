//
//  EnveloppeDocument.m
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 10/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "VSC::EnveloppeEditorDocument.h"
#import "NSApplication+VSCAdditions.h"


@implementation VSC::EnveloppeEditorDocument

static NSString* baseFilePath = nil;
static NSString* enveloppeBaseFilePath = nil;

-(id) init {
	
	if ((self = [super init])) {
		//[self createDefaultEnveloppe];
	}
	
	return self;
	
}

#pragma mark - NSDocument Methods

/*
- (NSString *)windowNibName {
    // Implement this to return a nib to load OR implement -makeWindowControllers to manually create your controllers.
    return @"VSC::EnveloppeDocument";
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

#pragma mark Window Controllers

-(void) makeWindowControllers {
	
	NSLog(@"%@ windowControllers are %@", self, [self windowControllers]);
	
	NSLog(@"%@ makeWindowControllers", self);
	
#if 0
	enveloppeEditorWindowController = [[VSC::EnveloppeEditorWindowController alloc] 
									   initWithWindowNibName:@"VSC::EnveloppeEditorWindowController"];
	[self addWindowController:enveloppeEditorWindowController];
	NSLog(@"%@ added window controller %@", self, enveloppeEditorWindowController);
	[enveloppeEditorWindowController release];
	[enveloppeEditorWindowController.loadedTextField setStringValue:@"LOADED WITHOUT CALLBACKS"];
#endif
    
    midiTestWindowController = [[VSCMIDITestWindowController alloc] 
                                initWithWindowNibName:@"VSCMIDITestWindowController"];
    [self addWindowController:midiTestWindowController];
	NSLog(@"%@ added window controller %@", self, midiTestWindowController);
	
	NSLog(@"%@ windowControllers are %@", self, [self windowControllers]);
	
}

- (void)windowControllerWillLoadNib:(NSWindowController *)windowController {
	NSLog(@"%@ window %@ windowControllerWillLoadNib", self, windowController);
	if (windowController == enveloppeEditorWindowController) {
		//[enveloppeEditorWindowController.loadedTextField setStringValue:@"LOADED WILL LOAD FROM DOCUMENT"];
	}
}


- (void)windowControllerDidLoadNib:(NSWindowController *)windowController {
	NSLog(@"%@ window %@ windowControllerDidLoadNib", self, windowController);
	if (windowController == enveloppeEditorWindowController) {
		//[enveloppeEditorWindowController.loadedTextField setStringValue:@"LOADED DID LOAD FROM DOCUMENT"];
	}
}

#pragma mark - Enveloppe Base File Paths


-(NSString*) baseFilePath {
	
	if (!baseFilePath) {
		@synchronized(self) {
			baseFilePath = [[[NSApplication sharedApplication] applicationLibraryDirectory] 
							 stringByAppendingPathComponent:@"VSC"];
		}
	}
	
	if (![[NSFileManager defaultManager] fileExistsAtPath:baseFilePath]) {
		[[NSFileManager defaultManager] createDirectoryAtPath:baseFilePath 
								  withIntermediateDirectories:YES attributes:nil error:nil];
	}
	
	return baseFilePath;
}

-(NSString*) enveloppeBaseFilePath {
	
	if (!enveloppeBaseFilePath) {
		@synchronized(self) {
			enveloppeBaseFilePath = [[self baseFilePath] stringByAppendingPathComponent:@"Enveloppes"];
		}
	}
	
	if (![[NSFileManager defaultManager] fileExistsAtPath:enveloppeBaseFilePath]) {
		[[NSFileManager defaultManager] createDirectoryAtPath:enveloppeBaseFilePath 
								  withIntermediateDirectories:YES attributes:nil error:nil];
	}
	
	return enveloppeBaseFilePath;
}

#pragma mark - Enveloppe Access/Creation/Add/Remove

-(void) createDefaultEnveloppe {
	
	VSC::Enveloppe::SPtr enveloppe = VSC::Enveloppe::SPtr(new VSC::Enveloppe());
	
	enveloppe->addPoint(VSC::EnveloppePoint::SPtr(new VSC::EnveloppePoint(0.0, 0.0)));
	enveloppe->addPoint(VSC::EnveloppePoint::SPtr(new VSC::EnveloppePoint(0.0, 1.0)));
	enveloppe->addPoint(VSC::EnveloppePoint::SPtr(new VSC::EnveloppePoint(0.0, 2.0)));
	enveloppe->addPoint(VSC::EnveloppePoint::SPtr(new VSC::EnveloppePoint(0.0, 3.0)));
	
	enveloppe->setFilePath(std::string([DEFAULT_ENVELOPPE_RELATIVE_PATH cStringUsingEncoding:NSUTF8StringEncoding]));
	
	[self addEnveloppe:enveloppe];
	
}

-(void) addEnveloppe:(VSC::Enveloppe::SPtr)enveloppe {
	_mainEnveloppeList.push_back(enveloppe);
}

-(void) removeEnveloppe:(VSC::Enveloppe::SPtr)enveloppe {
	_mainEnveloppeList.remove(enveloppe);
}

-(EnvIter) beginMainEnveloppeListIterator {
	return _mainEnveloppeList.begin();
}

-(EnvIter) endMainEnveloppeListIterator {
	return _mainEnveloppeList.end();
}

-(VSC::Enveloppe::SPtr) defaultEnveloppe {
	std::string defaultRelativePath = std::string([DEFAULT_ENVELOPPE_RELATIVE_PATH cStringUsingEncoding:NSUTF8StringEncoding]);
	for (EnvIter envIt = [self beginMainEnveloppeListIterator]; envIt != [self endMainEnveloppeListIterator]; envIt++) {
		VSC::Enveloppe* enveloppe = (*envIt).get();
		std::string relativePath = enveloppe->getFilePath();
		if (relativePath.compare(defaultRelativePath) == 0) {
			return *envIt;
		}
	}
	return VSC::Enveloppe::SPtr();
}

@end
