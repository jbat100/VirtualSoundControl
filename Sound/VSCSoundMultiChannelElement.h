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

#include <list>
#include <vector>

/*
 *	Root class for source elements (deliberatly not dependent on STK for possible future,
 *	changes in the C++ sound engine)
 */

class VSCSynthMultiChannelElement {
	
	
public:
    
    VSCSynthMultiChannelElement();
    VSCSynthMultiChannelElement(unsigned int numberOfChannels);
    
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
    
    /*
     *  Lock number of channels.
     */
    void lockChannels(bool _lock);
    bool numberOfChannelsIsLocked(void);
    
    /*--------------------------------------------------------------*/
	
protected:
	
    bool _lockNumberOfChannels;
    std::vector<VSCSFloat> _channelLinearGains;
    unsigned int _numberOfChannels;
	
};



#endif