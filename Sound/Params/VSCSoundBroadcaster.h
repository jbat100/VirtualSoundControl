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
#include "VSCSoundParameterized.h"
#include "VSCSoundPropertized.h"
#include "VSCSoundElement.h"

#include <map>
#include <stack>
#include <set>
#include <string>

#include <boost/shared_ptr.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

/*
 *	Identifier containing a parameter key and element
 *	overriden < operator to be able to use as key in std::map
 */
struct VSCSParameterId {
	VSCSoundElementPtr element;
	VSCSParameter::Key key;
	bool operator<(const VSCSParameterId& paramId) const;
};

/* 
 *	Gave up on seperate index parameters ...
 */
struct VSCSIndexedParameterId {
	VSCSoundElementPtr element;
	VSCSParameter::Key key;
	unsigned int index;
	bool operator<(const VSCSIndexedParameterId& paramId) const;
};
 

/*
 *	Identifier containing a property key and element
 *	overriden < operator to be able to use as key in std::map
 */
struct VSCSPropertyId {
	VSCSoundElementPtr element;
	VSCSProperty::Key key;
	bool operator<(const VSCSPropertyId& propId) const { return element < propId.element; }
};

/*
 *	Class used by the broadcaster, as interface for forwarding parameter changes
 */

#define VSCSBroadcastListenerPtr    boost::shared_ptr<VSCSBroadcastListener>

class VSCSBroadcastListener {
	
public:
	
	virtual void parameterChanged(VSCSParameterId paramId, double value);
	virtual void indexedParameterChanged(VSCSIndexedParameterId paramId, double value);
	virtual void propertyChanged(VSCSPropertyId, std::string value);
	
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
	
	/*
	 *	Singleton instance 
	 */
	VSCSoundBroadcaster& sharedBroadcaster(void);
	
    void parameterChanged(VSCSParameterId, double value);
	void indexedParameterChanged(VSCSIndexedParameterId, double value);
    void propertyChanged(VSCSPropertyId, std::string value);
   
	void setBroadcastInterval(VSCSFloat interval);
	VSCSFloat getBroadcastInterval(void);
	
	void addBroadcastListener(VSCSBroadcastListenerPtr listener);
	void removeBroadcastListener(VSCSBroadcastListenerPtr listener);
	const std::set<VSCSBroadcastListenerPtr>& broadcastListeners(void);
	
	void startBroadcasting(void);
	void stopBroadcasting(void);
	
private:
	
	void broadcast(void);
	void cycleBroadcastAndSleepUntilStopped(void);
	
	boost::thread broadcastCycleThread;
	
	boost::mutex listenerMutex;
	std::set<VSCSBroadcastListenerPtr> _broadcastListeners;
	
	boost::mutex intervalMutex;
	VSCSFloat _broadcastInterval;
	
	boost::mutex updateMutex;
	std::map<VSCSParameterId, double> _parameterUpdateMap;
	std::map<VSCSIndexedParameterId, double> _indexedParameterUpdateMap;
	std::map<VSCSPropertyId, std::string> _propertyUpdateMap;
	
};

#endif

