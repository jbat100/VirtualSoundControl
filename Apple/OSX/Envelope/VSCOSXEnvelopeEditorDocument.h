//
//  EnvelopeDocument.h
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 10/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include <string>
#include <list>
#include <boost/shared_ptr.hpp>

#include "VSCEnvelope.h"
#include "VSCEnvelopePoint.h"

@class VSCOSXEnvelopeEditorWindowController;
@class VSCOSXMIDIWindowController;

#define DEFAULT_ENVELOPE_RELATIVE_PATH		@"Default"

@interface VSCOSXEnvelopeEditorDocument : NSDocument
{
	std::vector<VSC::Envelope::SPtr> _mainEnvelopeList;
}

@property (nonatomic, strong) VSCOSXEnvelopeEditorWindowController* envelopeEditorWindowController;
@property (nonatomic, strong) VSCOSXMIDIWindowController* midiTestWindowController;

/*
 *	Envelope Base File Paths
 */
-(NSString*) envelopeBaseFilePath;
-(NSString*) baseFilePath;

/*
 *	Envelope Access/Create/Add/Remove
 */

-(void) addEnvelope:(VSC::Envelope::SPtr)envelope;
-(void) removeEnvelope:(VSC::Envelope::SPtr)envelope;
-(EnvIter) beginMainEnvelopeListIterator;
-(EnvIter) endMainEnvelopeListIterator;
-(void) createDefaultEnvelope;
-(VSC::Envelope::SPtr) defaultEnvelope;

@end
