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

#define EnvIter				std::list<VSCEnveloppePtr>::iterator 
#define ConstEnvIter		std::list<VSCEnveloppePtr>::const_iterator 

@interface VSCEnveloppeEditorDocument : NSDocument {
	
	/*
	 *	Enveloppe mapped by pathname
	 */
	std::list<VSCEnveloppePtr> _mainEnveloppeMap;

}


@end
