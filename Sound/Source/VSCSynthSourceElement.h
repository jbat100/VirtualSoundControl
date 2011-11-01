//
//  VSCSynthSourceElement.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 9/26/11.
//  Copyright 2011 VSC. All rights reserved.
//

#ifndef _VSC_SYNTH_SOURCE_ELEMENT_
#define _VSC_SYNTH_SOURCE_ELEMENT_

#include "VSCSound.h"

#include <list>
#include <boost/shared_ptr.hpp>

#define VSCSynthSourceElementPtr    boost::shared_ptr<VSCSynthSourceElement>

#define ConstSynthSrcElemIter		std::list<VSCSynthSourceElementPtr>::const_iterator 
#define ConstRevSynthSrcElemIter	std::list<VSCSynthSourceElementPtr>::const_reverse_iterator 

#define SynthSrcElemIter			std::list<VSCSynthSourceElementPtr>::iterator 
#define RevSynthSrcElemIter			std::list<VSCSynthSourceElementPtr>::reverse_iterator 

/*
 *	Root class for source elements (deliberatly not dependent on STK for possible future,
 *	changes in the C++ sound engine)
 */

class VSCSynthSourceElement {
	
	
public:
	
	/* value between 0 and 1 */
	void setLinearGain(VSCSFloat g);
	VSCSFloat getLinearGet(void) const;
	
	/* value between 0 and minus infinity */
	void setDBGain(VSCSFloat g);	
	VSCSFloat getDBGain(void) const;
	
	void setOn(bool on);
	bool isOn(void) const;
	
	virtual std::string sourceTypeString(void);
	
	
protected:
	
	bool _isOn;
	VSCSFloat _linearGain; /* value between 0 and 1 */
	
};



#endif
