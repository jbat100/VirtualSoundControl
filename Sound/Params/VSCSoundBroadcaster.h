/*
 *  VSCSoundBroadcaster.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_SOUND_BROADCASTER_H_
#define _VSC_SOUND_BROADCASTER_H_

#include "VSCSound.h"
#include "VSCSoundParameters.h"
#include "VSCSoundProperties.h"
#include "VSCSoundParameterizedElement.h"
#include "VSCSoundPropertizedElement.h"

#ifdef __APPLE__
#include "VSCSoundBroadcastAppleRelay.h"
#endif

#include <map>
#include <string>

/*
 *	Class used by the broadcaster, as interface for forwarding parameter changes
 */

class VSCSParameterListener {
	
public:
	
	void addParameterizedElement(VSCSoundParameterizedElement* element);
	void removeParameterizedElement(VSCSoundParameterizedElement* element);
	std::set<VSCSoundParameterizedElement*> getParameterizedElements(void);
	
	void addParameterKey(VSCSParameter::Key k);
	void removeParameterKey(VSCSParameter::Key k);
	std::set<VSCSParameter::Key> getParameterKeys(void);
	
	void setMinimumRate(VSCSFloat r);
	VSCSFloat getMinimumRate(void);
	
protected:
	
	/* The elements for which the listener wants to receive events */
	std::set<VSCSoundParameterizedElement*> _parameterizedElements;
	
	/* The keys for which the listener wants to recieve events */
	std::set<VSCSParameter::Key> _parameterKeys;
	
	/* The minimum rate at which the listener should receive updates */
	VSCSFloat _minimumRate;
	
};

/*
 *	Class used by the broadcaster, as interface for forwarding parameter changes
 */

class VSCSPropertyListener {
	
public:
	
	void addPropertizedElement(VSCSoundPropertizedElement* element);
	void removePropertizedElement(VSCSoundPropertizedElement* element);
	std::set<VSCSoundPropertizedElement*> getPropertizedElements(void);
	
	void addPropertyKey(VSCSProperty::Key k);
	void removePropertyKey(VSCSProperty::Key k);
	std::set<VSCSProperty::Key> getPropertyKeys(void);
	
	void setMinimumRate(VSCSFloat r);
	VSCSFloat getMinimumRate(void);
	
protected:
	
	/* The elements for which the listener wants to receive events */
	std::set<VSCSoundPropertizedElement*> _propertizedElements;
	
	/* The keys for which the listener wants to recieve events */
	std::set<VSCSProperty::Key> _propertyKeys;
	
	/* The minimum rate at which the listener should receive updates */
	VSCSFloat _minimumRate;
	
};



class VSCSoundBroadcaster {
	
public:
	
    void parameterChanged(VSCSoundParameterizedElement* element, VSCSParameter::Key, double value);
    void propertyChanged(VSCSoundPropertizedElement* element, VSCSProperty::Key, std::string value);
   
#ifdef __APPLE__
    VSCSoundBroadcastAppleRelay& getAppleRelay(void);
#endif
	
protected:
    
#ifdef __APPLE__
    VSCSoundBroadcastAppleRelay _appleRelay;
#endif
	
	std::set<VSCSParameterListener*> _parameterListeners;
    std::set<VSCSPropertyListener*> _propertyListeners;
	
	
};

#endif

