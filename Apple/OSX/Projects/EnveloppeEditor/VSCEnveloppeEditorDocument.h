//
//  EnveloppeDocument.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 10/03/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#include <string>
#include <list>
#include <boost/shared_ptr.hpp>

#include "VSC::Enveloppe.h"
#include "VSC::EnveloppePoint.h"

#import "VSC::EnveloppeEditorWindowController.h"
#import "VSCMIDITestWindowController.h"

#define DEFAULT_ENVELOPPE_RELATIVE_PATH		@"Default"

@class VSC::EnveloppeEditorWindowController;

#define EnvIter				std::list<VSC::Enveloppe::SPtr>::iterator 
#define ConstEnvIter		std::list<VSC::Enveloppe::SPtr>::const_iterator 

@interface VSC::EnveloppeEditorDocument : NSDocument {
	
	std::list<VSC::Enveloppe::SPtr> _mainEnveloppeList;
	
	VSC::EnveloppeEditorWindowController* enveloppeEditorWindowController;
    
    VSCMIDITestWindowController* midiTestWindowController;

}

/*
 *	Enveloppe Base File Paths
 */
-(NSString*) enveloppeBaseFilePath;
-(NSString*) baseFilePath;

/*
 *	Enveloppe Access/Create/Add/Remove
 */

-(void) addEnveloppe:(VSC::Enveloppe::SPtr)enveloppe;
-(void) removeEnveloppe:(VSC::Enveloppe::SPtr)enveloppe;
-(EnvIter) beginMainEnveloppeListIterator;
-(EnvIter) endMainEnveloppeListIterator;
-(void) createDefaultEnveloppe;
-(VSC::Enveloppe::SPtr) defaultEnveloppe;

@end
