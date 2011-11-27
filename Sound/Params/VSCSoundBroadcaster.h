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

#include <map>
#include <stack>
#include <set>
#include <string>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

/*
 *	Identifier containing a parameter key and element
 *	overriden < operator to be able to use as key in std::map
 */
struct VSCSParameterId {
	VSCSoundParameterizedElement* element;
	VSCSParameter::Key key;
	bool operator<(const VSCSParameterId& paramId) const { return element < paramId.element; }
};
struct VSCSParameterUpdate {
	VSCSParameterId paramId;
	double value;
};

/*
 *	Identifier containing a property key and element
 *	overriden < operator to be able to use as key in std::map
 */
struct VSCSPropertyId {
	VSCSoundPropertizedElement* element;
	VSCSProperty::Key key;
	bool operator<(const VSCSPropertyId& propId) const { return element < propId.element; }
};
struct VSCSPropertyUpdate {
	VSCSPropertyId propId;
	std::string value;
}




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
    
	
    virtual bool interestedInParameterId(VSCSParameterId paramId);
	virtual void parameterChanged(VSCSoundParameterizedElement* element, VSCSParameter::Key, double value);
	
protected:
	
	/* The elements for which the listener wants to receive events */
	std::set<VSCSoundParameterizedElement*> _parameterizedElements;
	
	/* The keys for which the listener wants to recieve events */
	std::set<VSCSParameter::Key> _parameterKeys;
	
};

/*
 *	Class used by the broadcaster, as interface for forwarding parameter changes
 */

class VSCSPropertyListener {
	
public:
	
	/*
	 *	Determine the elements for which the listener is interested 
	 */
	void addPropertizedElement(VSCSoundPropertizedElement* element);
	void removePropertizedElement(VSCSoundPropertizedElement* element);
	std::set<VSCSoundPropertizedElement*> getPropertizedElements(void);
	
	/*
	 *	Determine the keys for which the listener is interested
	 */
	void addPropertyKey(VSCSProperty::Key k);
	void removePropertyKey(VSCSProperty::Key k);
	std::set<VSCSProperty::Key> getPropertyKeys(void);
    
    /*
     *  Ask whether the listener is interested by a given combination of element/key
     */
    virtual bool interestedInPropertyId(VSCSPropertyId propId);
	virtual void propertyChanged(VSCSoundPropertizedElement* element, VSCSProperty::Key, std::string value);
	
protected:
	
	/* The elements for which the listener wants to receive events */
	std::set<VSCSoundPropertizedElement*> _propertizedElements;
	
	/* The keys for which the listener wants to recieve events */
	std::set<VSCSProperty::Key> _propertyKeys;
	
};

/*
 *	This class aims to provide updates to interface elements at a rate far lower
 *	than audio sampling rates (around 30Hz as opposed to 48KHz). To save wasting many 
 *	many cycles on pointlessly frequent updates.
 *
 *	Parameter/property
 *	updates are stored in a map waiting for the next broadcast. After the broadcast
 *	the updates maps are cleared waiting for new updates.
 */

class VSCSoundBroadcaster {
	
public:
	
	VSCSoundBroadcaster();
	~VSCSoundBroadcaster();
	
    void parameterChanged(VSCSoundParameterizedElement* element, VSCSParameter::Key, double value);
    void propertyChanged(VSCSoundPropertizedElement* element, VSCSProperty::Key, std::string value);
   
	void setBroadcastInterval(VSCSFloat interval);
	VSCSFloat getBroadcastInterval(void);
	
	void addParameterListener(VSCSParameterListener* listener);
	void removeParameterListener(VSCSParameterListener* listener);
	
	void addPropertyListener(VSCSPropertyListener* listener);
	void removePropertyListener(VSCSPropertyListener* listener);
	
	void startBroadcasting(void);
	void stopBroadcasting(void);
	
private:
	
	void broadcast(void);
	
	void cycleBroadcastAndSleepUntilStopped(void);
	
	boost::thread broadcastCycleThread;
	
	boost::mutex listenerMutex;
	std::set<VSCSParameterListener*> _parameterListeners;
    std::set<VSCSPropertyListener*> _propertyListeners;
	
	boost::mutex intervalMutex;
	VSCSFloat _broadcastInterval;
	
	boost::mutex updateMutex;
	std::map<VSCSParameterId, double> _parameterUpdateMap;
	std::map<VSCSPropertyId, std::string> _propertyUpdateMap;
	
};

#endif

