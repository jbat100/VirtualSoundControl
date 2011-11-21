/*
 *  VSCSoundParameterizedElement.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_SOUND_PARAMETERIZED_ELEMENT_
#define _VSC_SOUND_PARAMETERIZED_ELEMENT_

#include "VSCSound.h"
#include "VSCSoundParameters.h"

#include <set>
#include <boost/shared_ptr.hpp>

/*
 *	Smart pointer and iterator convenience defines
 */

#define VSCSoundParameterizedElementPtr    boost::shared_ptr<VSCSoundParameterizedElement>


/*
 *	Root class for adding/removing setting/getting parameter values
 */

class VSCSoundParameterizedElement {
	
	
public:
	
	void addParameterKey(VSCSParameter::Key k);
	void removeParameterKey(VSCSParameter::Key k);
	std::set<VSCSParameter::Key>& getParameterKeys(void);
	
	virtual double getValueForParameterWithKey(VSCSParameter::Key key);
	virtual void setValueForParameterWithKey(double value, VSCSParameter::Key key);
	
protected:
	
    std::set<VSCSParameter::Key> _parameterKeys;
	
};

/*
 *	Class used by the broadcaster, as interface for forwarding parameter changes
 */

class VSCSParameterListener {
	
public:
	
	void addParameterizedElement(VSCSoundParameterizedElement* element);
	void removeParameterizedElement(VSCSoundParameterizedElement* element);
	std::set<VSCSoundParameterizedElement*> getParameterizedElements;
	
	void addParameterKey(VSCSParameter::Key k);
	void removeParameterKey(VSCSParameter::Key k);
	std::set<VSCSParameter::Key> getParameterKeys;
	
	void setMinimumRate(VSCSFloat r);
	VSCSFloat getMinimumRate(void);
	
protected:
	
	/* The elements for which the listener wants to receive events */
	std::set<VSCSoundParameterizedElement*> _parameterizedElements;
	
	/* The keys for which the listener wants to recieve events */
	std::set<VSCSParameter::Key> _parameterKeys;
	
	/* The minimum rate at which the listener should receive updates */
	VSCSFloat minimumRate;
	
};


#endif