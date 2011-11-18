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
    VSCSoundMultiChannelElement(unsigned int numberOfChannels);
    
    /*--------------------------------------------------------------*/
    
    /*
     *  METHODS IN THIS SECTION CAN BE CALLED FROM THE CONSTRUCTOR AND SO SHOULD
     *  NOT BE VIRTUAL
     */
    
    void setNumberOfChannels(unsigned int numberOfChannels);
    unsigned int getNumberOfChannels(void);
	
    /*
     *  These methods set the gain equally for the channels.
     */
	/* value between 0 and 1 */
	void setLinearGain(VSCSFloat g);
	/* value between 0 and minus infinity */
	void setDBGain(VSCSFloat g);	
    
    /*
     *  Set/get individual gains for each channel.
     */
    void setLinearGains(std::vector<VSCSFloat>& channelGains);
    void getLinearGains(std::vector<VSCSFloat>& channelGains) const;
    void setDBGains(std::vector<VSCSFloat>& channelDBGains);
    void getDBGains(std::vector<VSCSFloat>& channelDBGains) const;
    
    void setLinearGainAtChannel(VSCSFloat gain, unsigned int channel);
    VSCSFloat getLinearGainAtChannel(unsigned int channel);
	
	/*
	 *	Not sure why this would be useful outside, but don't see any reason to make it private...
	 */
	VSCSFloat averageLinearGain(void);
    VSCSFloat averageDBGain(void);
    
    /*
     *  Lock number of channels.
     */
    void lockChannels(bool _lock);
    bool numberOfChannelsIsLocked(void);
	
	/*
	 *	Don't actually think these are necessary, subclasses can see if they want the parameter,
	 *	and pass it upwards if they don't
	 */
	//bool isMultiChannelElementParameterKey(VSCSParameter::Key k);
	//bool isMultiChannelElementPropertyKey(VSCSProperty::Key k);
    
    /*--------------------------------------------------------------*/
	
	/*
	 * These functions are inherited from VSCSoundPropertizedElement and VSCSoundParameterizedElement
	 */
	
	virtual double getValueForParameterWithKey(VSCSParameter::Key key);
	virtual void setValueForParameterWithKey(double value, VSCSParameter::Key key);
	
	virtual double getValueForPropertyWithKey(VSCSProperty::Key key);
	virtual void setValueForPropertyWithKey(double value, VSCSProperty::Key key);
	
	/*--------------------------------------------------------------*/
	
	
protected:
	
    bool _lockNumberOfChannels;
    std::vector<VSCSFloat> _channelLinearGains;
    unsigned int _numberOfChannels;
	
private:
	
	void addMultiChannelParameters(void);
	void addMultiChannelProperties(void);
    void resetMultiChannelParameters(void);
	
};



#endif