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
#include "VSCSound.h"

/*
 *	STK stuff
 *
 *	Using the stk:: namespace scoping explicitely to avoid namespace pollution
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
	stk::StkFloat lastOut(void) const {return lastFrame_[0];};
	
	//! Compute and return one output sample.
	stk::StkFloat tick(void);
	
	//! Fill a channel of the StkFrames object with computed outputs.
	/*!
	 The \c channel argument must be less than the number of
	 channels in the StkFrames argument (the first channel is specified
	 by 0).  However, range checking is only performed if _STK_DEBUG_
	 is defined during compilation, in which case an out-of-range value
	 will trigger an StkError exception.
	 */
	stk::StkFrames& tick(stk::StkFrames& frames, unsigned int channel = 0);
	
	/* FIRE !!! */
	virtual void fire(void);
	virtual void fireAfterInterval(VSCSFloat time);
	virtual void setCurrentTime(VSCSFloat currentTime);
	
	
protected:
	
	void enveloppeChangedBetweenEnveloppePoints(VSCEnveloppePointPtr begin, VSCEnveloppePointPtr end);
	void enveloppeChangedBetweenEnveloppePointAndNext(VSCEnveloppePointPtr point);
	
	/*
	 *	Update table according to enveloppe point changes. The update calls can be made in
	 *	two flavors, by passing EnvPoint pointers or iterators (if they are known this is faster).
	 */
	void checkTableSize(void);
	void updateTable(void);
	void updateTableBetweenEnveloppePoints(VSCEnveloppePointPtr begin, VSCEnveloppePointPtr end);
	void updateTableBetweenEnveloppePoints(ConstEnvPntIter beginIt, ConstEnvPntIter endIt);
	void updateTableBetweenEnveloppePointAndNext(VSCEnveloppePointPtr point);
	void updateTableBetweenEnveloppePointAndNext(ConstEnvPntIter point);
	unsigned int upperTableIndexForTime(VSCSFloat time); // closest table index after or == time 
	unsigned int lowerTableIndexForTime(VSCSFloat time); // closest table index before or == time 
	
	/*
	 *	Deal with sample rate changes
	 */
	void sampleRateChanged(stk::StkFloat newRate, stk::StkFloat oldRate);
	
	/*
	 *	table of samples with fixed time interval (size is proportional to the
	 *	time duration of the enveloppe.
	 */ 
	static stk::StkFrames table_;
	
	/*
	 *	current sample is a float, could be an int really, 
	 */
	stk::StkFloat currentSample_;

};

inline stk::StkFloat VSCSTKEnveloppe::tick(void)
{
	currentSample_ += 1.0;
	unsigned int index = (unsigned int)currentSample_;
	if (index >= table_.frames())
		lastFrame_[0] = table_[(unsigned int)currentSample_];
	else 
		lastFrame_[0] = table_[(unsigned int)(table_.frames()-1)];
	return lastFrame_[0];
}

inline stk::StkFrames& VSCSTKEnveloppe::tick(stk::StkFrames& frames, unsigned int channel)
{
#if defined(_STK_DEBUG_)
	if ( channel >= frames.channels() ) {
		errorString_ << "VSCSTKEnveloppe::tick(): channel and StkFrames arguments are incompatible!";
		handleError(StkError::FUNCTION_ARGUMENT);
	}
#endif
	
	stk::StkFloat *samples = &frames[channel];
	stk::StkFloat tmp = 0.0;
	
	unsigned int hop = frames.channels();
	for (unsigned int i=0; i<frames.frames(); i++, samples += hop) {
		currentSample_ += 1.0;
		unsigned int index = (unsigned int)currentSample_;
		if (index >= table_.frames())
			tmp = table_[(unsigned int)currentSample_];
		else 
			tmp = table_[(unsigned int)(table_.frames()-1)];
		*samples = tmp;
	}
	
	lastFrame_[0] = tmp;
	return frames;
}

#endif // _VSCSTK_ENVELOPPE_H_
