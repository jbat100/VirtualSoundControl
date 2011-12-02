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
#include "VSCSoundElement.h"

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

class VSCSoundMultiChannelElement : public VSCSoundElement {
	
	
public:
    
    VSCSoundMultiChannelElement();
	
	bool numberOfChannelsIsLocked(void);
	void lockChannels(bool _lock);
	VSCSFloat averageLinearGain(void);
	VSCSFloat averageDBGain(void);
    
    /*--------------------------------------------------------------*/
	
	/*
	 *	Make setter method virtual to allow subclasses to automatically update their engines
	 */
	unsigned int getNumberOfChannels(void);
	virtual void setNumberOfChannels(unsigned int c);
	
	/*
	 * These functions are inherited from VSCSoundPropertizedElement and VSCSoundParameterizedElement
	 */
	
	virtual double getValueForParameterWithKey(VSCSParameter::Key key);
	virtual void setValueForParameterWithKey(double value, VSCSParameter::Key key);
	
	virtual std::string getValueForPropertyWithKey(VSCSProperty::Key key);
	virtual void setValueForPropertyWithKey(std::string value, VSCSProperty::Key key);
	
	/*--------------------------------------------------------------*/
	

	
protected:
	
    /*
     *  Provide const ref for fast access in subclasses for audio processing
     */
    const std::vector<VSCSFloat>& getChannelLinearGains(void);
    
private:

    static const std::string kSoundMultiChannelType;
    
    std::vector<VSCSFloat> _channelLinearGains;
    unsigned int _numberOfChannels;
    bool _lockNumberOfChannels;
    void resetMultiChannelParameters(void);
    
	
};



#endif