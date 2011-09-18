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
	
public:
	
	//! Clear output and reset time pointer to zero.
	void reset(void);
	
	//! Return the last computed output value.
	StkFloat lastOut(void) const {return lastFrame_[0];};
	
	//! Compute and return one output sample.
	StkFloat tick(void);
	
	//! Fill a channel of the StkFrames object with computed outputs.
	/*!
	 The \c channel argument must be less than the number of
	 channels in the StkFrames argument (the first channel is specified
	 by 0).  However, range checking is only performed if _STK_DEBUG_
	 is defined during compilation, in which case an out-of-range value
	 will trigger an StkError exception.
	 */
	StkFrames& tick(StkFrames& frames, unsigned int channel = 0);
	
protected:
	
	/*
	 *	Update table according to enveloppe point changes
	 */
	void updateTable(void);
	void updateTableBetweenEnveloppePoints(VSCEnveloppePointPtr begin, VSCEnveloppePointPtr end);
	void updateTableBetweenEnveloppePointAndNext(VSCEnveloppePointPtr point);
	
	/*
	 *	Deal with sample rate changes
	 */
	void sampleRateChanged( StkFloat newRate, StkFloat oldRate );
	
	/*
	 *	table of samples with fixed time interval (size is proportional to the
	 *	time duration of the enveloppe.
	 */ 
	static StkFrames table_;
	
	/*
	 *	current sample is a float, could be an int really, 
	 */
	StkFloat currentSample_;

};

#endif