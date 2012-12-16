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

#import "VSC::EnvelopeEditorWindowController.h"
#import "VSCMIDITestWindowController.h"

#define DEFAULT_ENVELOPE_RELATIVE_PATH		@"Default"

@class VSC::EnvelopeEditorWindowController;

#define EnvIter				std::list<VSC::Envelope::SPtr>::iterator 
#define ConstEnvIter		std::list<VSC::Envelope::SPtr>::const_iterator 

@interface VSC::EnvelopeEditorDocument : NSDocument {
	
	std::list<VSC::Envelope::SPtr> _mainEnvelopeList;
	
	VSC::EnvelopeEditorWindowController* envelopeEditorWindowController;
    
    VSCMIDITestWindowController* midiTestWindowController;

}

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
