//
//  EnvelopeDocument.m
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 10/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "VSC::EnvelopeEditorDocument.h"
#import "NSApplication+VSCAdditions.h"


@implementation VSC::EnvelopeEditorDocument

static NSString* baseFilePath = nil;
static NSString* envelopeBaseFilePath = nil;

-(id) init {
	
	if ((self = [super init])) {
		//[self createDefaultEnvelope];
	}
	
	return self;
	
}

#pragma mark - NSDocument Methods

/*
- (NSString *)windowNibName {
    // Implement this to return a nib to load OR implement -makeWindowControllers to manually create your controllers.
    return @"VSC::EnvelopeDocument";
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
	envelopeEditorWindowController = [[VSC::EnvelopeEditorWindowController alloc] 
									   initWithWindowNibName:@"VSC::EnvelopeEditorWindowController"];
	[self addWindowController:envelopeEditorWindowController];
	NSLog(@"%@ added window controller %@", self, envelopeEditorWindowController);
	[envelopeEditorWindowController release];
	[envelopeEditorWindowController.loadedTextField setStringValue:@"LOADED WITHOUT CALLBACKS"];
#endif
    
    midiTestWindowController = [[VSCMIDITestWindowController alloc] 
                                initWithWindowNibName:@"VSCMIDITestWindowController"];
    [self addWindowController:midiTestWindowController];
	NSLog(@"%@ added window controller %@", self, midiTestWindowController);
	
	NSLog(@"%@ windowControllers are %@", self, [self windowControllers]);
	
}

- (void)windowControllerWillLoadNib:(NSWindowController *)windowController {
	NSLog(@"%@ window %@ windowControllerWillLoadNib", self, windowController);
	if (windowController == envelopeEditorWindowController) {
		//[envelopeEditorWindowController.loadedTextField setStringValue:@"LOADED WILL LOAD FROM DOCUMENT"];
	}
}


- (void)windowControllerDidLoadNib:(NSWindowController *)windowController {
	NSLog(@"%@ window %@ windowControllerDidLoadNib", self, windowController);
	if (windowController == envelopeEditorWindowController) {
		//[envelopeEditorWindowController.loadedTextField setStringValue:@"LOADED DID LOAD FROM DOCUMENT"];
	}
}

#pragma mark - Envelope Base File Paths


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

-(NSString*) envelopeBaseFilePath {
	
	if (!envelopeBaseFilePath) {
		@synchronized(self) {
			envelopeBaseFilePath = [[self baseFilePath] stringByAppendingPathComponent:@"Envelopes"];
		}
	}
	
	if (![[NSFileManager defaultManager] fileExistsAtPath:envelopeBaseFilePath]) {
		[[NSFileManager defaultManager] createDirectoryAtPath:envelopeBaseFilePath 
								  withIntermediateDirectories:YES attributes:nil error:nil];
	}
	
	return envelopeBaseFilePath;
}

#pragma mark - Envelope Access/Creation/Add/Remove

-(void) createDefaultEnvelope {
	
	VSC::Envelope::SPtr envelope = VSC::Envelope::SPtr(new VSC::Envelope());
	
	envelope->addPoint(VSC::EnvelopePoint::SPtr(new VSC::EnvelopePoint(0.0, 0.0)));
	envelope->addPoint(VSC::EnvelopePoint::SPtr(new VSC::EnvelopePoint(0.0, 1.0)));
	envelope->addPoint(VSC::EnvelopePoint::SPtr(new VSC::EnvelopePoint(0.0, 2.0)));
	envelope->addPoint(VSC::EnvelopePoint::SPtr(new VSC::EnvelopePoint(0.0, 3.0)));
	
	envelope->setFilePath(std::string([DEFAULT_ENVELOPE_RELATIVE_PATH cStringUsingEncoding:NSUTF8StringEncoding]));
	
	[self addEnvelope:envelope];
	
}

-(void) addEnvelope:(VSC::Envelope::SPtr)envelope {
	_mainEnvelopeList.push_back(envelope);
}

-(void) removeEnvelope:(VSC::Envelope::SPtr)envelope {
	_mainEnvelopeList.remove(envelope);
}

-(EnvIter) beginMainEnvelopeListIterator {
	return _mainEnvelopeList.begin();
}

-(EnvIter) endMainEnvelopeListIterator {
	return _mainEnvelopeList.end();
}

-(VSC::Envelope::SPtr) defaultEnvelope {
	std::string defaultRelativePath = std::string([DEFAULT_ENVELOPE_RELATIVE_PATH cStringUsingEncoding:NSUTF8StringEncoding]);
	for (EnvIter envIt = [self beginMainEnvelopeListIterator]; envIt != [self endMainEnvelopeListIterator]; envIt++) {
		VSC::Envelope* envelope = (*envIt).get();
		std::string relativePath = envelope->getFilePath();
		if (relativePath.compare(defaultRelativePath) == 0) {
			return *envIt;
		}
	}
	return VSC::Envelope::SPtr();
}

@end
