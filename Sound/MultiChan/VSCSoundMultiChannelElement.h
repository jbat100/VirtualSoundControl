/*
 *  VSCSoundMultiChannelElement.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 05/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_SYNTH_MULTI_CHANNEL_ELEMENT_
#define _VSC_SYNTH_MULTI_CHANNEL_ELEMENT_

#include "VSCSound.h"
#include "VSCSoundParameters.h"
#include "VSCSoundProperties.h"
#include "VSCSoundParameterizedElement.h"
#include "VSCSoundPropertizedElement.h"

#include <list>
#include <vector>
#include <set>
#include <boost/shared_ptr.hpp>

/*
 *	Smart pointer and iterator convenience defines
 */

#define VSCSoundMultiChannelElementPtr    boost::shared_ptr<VSCSoundMultiChannelElement>


/*
 *	Root class for source elements (deliberatly not dependent on STK for possible future,
 *	changes in the C++ sound engine)
 */

class VSCSoundMultiChannelElement : public VSCSoundPropertizedElement , public VSCSoundParameterizedElement {
	
	
public:
    
    VSCSoundMultiChannelElement();
    
    /*--------------------------------------------------------------*/
	
	/*
	 * These functions are inherited from VSCSoundPropertizedElement and VSCSoundParameterizedElement
	 */
	
	virtual double getValueForParameterWithKey(VSCSParameter::Key key);
	virtual void setValueForParameterWithKey(double value, VSCSParameter::Key key);
	
	virtual double getValueForPropertyWithKey(VSCSProperty::Key key);
	virtual void setValueForPropertyWithKey(double value, VSCSProperty::Key key);
	
	/*--------------------------------------------------------------*/
	

	
private:
	
	bool _lockNumberOfChannels;
    std::vector<VSCSFloat> _channelLinearGains;
    unsigned int _numberOfChannels;
	
    void resetMultiChannelParameters(void);
	VSCSFloat VSCSoundMultiChannelElement::averageLinearGain(void);
	VSCSFloat VSCSoundMultiChannelElement::averageDBGain(void);
	void lockChannels(bool _lock);
    bool numberOfChannelsIsLocked(void);
	
};



#endif