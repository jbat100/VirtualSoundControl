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

#include "VSCEnveloppe.h"
#include "VSCEnveloppePoint.h"

#import "VSCEnveloppeEditorWindowController.h"

@class VSCEnveloppeEditorWindowController;

#define EnvIter				std::list<VSCEnveloppePtr>::iterator 
#define ConstEnvIter		std::list<VSCEnveloppePtr>::const_iterator 

@interface VSCEnveloppeEditorDocument : NSDocument {
	
	std::list<VSCEnveloppePtr> _mainEnveloppeList;
	
	VSCEnveloppeEditorWindowController* enveloppeEditorWindowController;

}

/*
 *	Enveloppe Base File Paths
 */
-(NSArray*) enveloppeBaseFilePaths;

/*
 *	Enveloppe Access/Create/Add/Remove
 */

-(void) addEnveloppe:(VSCEnveloppePtr)enveloppe;
-(void) removeEnveloppe:(VSCEnveloppePtr)enveloppe;
-(ConstEnvIter) beginMainEnveloppeListIterator;
-(ConstEnvIter) endMainEnveloppeListIterator;
-(void) createDefaultEnveloppe;

@end
