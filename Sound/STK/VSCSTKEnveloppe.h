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
	
	/* FIRE !!! */
	virtual void fire(void);
	virtual void fireAfterInterval(double time);
	
	
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
	unsigned int upperTableIndexForTime(double time); // closest table index after or == time 
	unsigned int lowerTableIndexForTime(double time); // closest table index before or == time 
	
	/*
	 *	Deal with sample rate changes
	 */
	void sampleRateChanged(StkFloat newRate, StkFloat oldRate);
	
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

inline StkFloat VSCSTKEnveloppe::tick(void)
{
	// Check limits of time address ... if necessary, recalculate modulo
	// TABLE_SIZE.
	while ( time_ < 0.0 )
		time_ += TABLE_SIZE;
	while ( time_ >= TABLE_SIZE )
		time_ -= TABLE_SIZE;
	
	iIndex_ = (unsigned int) time_;
	alpha_ = time_ - iIndex_;
	StkFloat tmp = table_[ iIndex_ ];
	tmp += ( alpha_ * ( table_[ iIndex_ + 1 ] - tmp ) );
	
	// Increment time, which can be negative.
	time_ += rate_;
	
	lastFrame_[0] = tmp;
	return lastFrame_[0];
}

inline StkFrames& VSCSTKEnveloppe::tick(StkFrames& frames, unsigned int channel)
{
#if defined(_STK_DEBUG_)
	if ( channel >= frames.channels() ) {
		errorString_ << "VSCSTKEnveloppe::tick(): channel and StkFrames arguments are incompatible!";
		handleError(StkError::FUNCTION_ARGUMENT);
	}
#endif
	
	StkFloat *samples = &frames[channel];
	StkFloat tmp = 0.0;
	
	unsigned int hop = frames.channels();
	for ( unsigned int i=0; i<frames.frames(); i++, samples += hop ) {
		
		// Check limits of time address ... if necessary, recalculate modulo
		// TABLE_SIZE.
		while ( time_ < 0.0 )
			time_ += TABLE_SIZE;
		while ( time_ >= TABLE_SIZE )
			time_ -= TABLE_SIZE;
		
		iIndex_ = (unsigned int) time_;
		alpha_ = time_ - iIndex_;
		tmp = table_[ iIndex_ ];
		tmp += ( alpha_ * ( table_[ iIndex_ + 1 ] - tmp ) );
		*samples = tmp;
		
		// Increment time, which can be negative.
		time_ += rate_;
	}
	
	lastFrame_[0] = tmp;
	return frames;
}

#endif // _VSCSTK_ENVELOPPE_H_
