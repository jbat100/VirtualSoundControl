/*
 *  VSCSTKEnveloppe.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 15/09/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSCSTK_ENVELOPPE_H_
#define _VSCSTK_ENVELOPPE_H_

/*
 *	VSC stuff
 */
#include "VSCEnveloppe.h"
#include "VSCEnveloppePoint.h"

/*
 *	STK stuff
 */
#include <Generator.h>

/*
 *	STL and BOOST stuff
 */
#include <list>
#include <string>
#include <boost/shared_ptr.hpp>

#define VSCSTKEnveloppePtr		boost::shared_ptr<VSCSTKEnveloppe>

class VSCSTKEnveloppe : public VSCEnveloppe, public stk::Generator {
	
private:
	
	double _tickTimeInterval;
	double _currentTime;
	ConstEnvPntIter _currentPntIter;
	
public:
	
	// tick function


};

#endif