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
#include <vector>
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
    
    VSCSynthSourceElement();
    VSCSynthSourceElement(unsigned int numberOfChannels);
    
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
     *  Turns off all processing.
     */
	void setOn(bool on);
	bool isOn(void) const;
    
    /*
     *  Lock number of channels.
     */
    void lockChannels(bool _lock);
    bool numberOfChannelsIsLocked(void);
    
    /*--------------------------------------------------------------*/
    
    /*
     *  Perform initialization after constructor (seperate so that it can be virtual).
     */
    virtual void initialize(void);
    
    /*
     *  Perform necessary setup operations after:
     *      - a change in number of channels.
     */
    virtual void updateSoundEngine(void);
    
    /*
     *  A human readable identifier for the source element.
     */
	virtual std::string sourceTypeString(void);
	
	
protected:
	
	bool _isOn;
    bool _lockNumberOfChannels;
    std::vector<VSCSFloat> _channelLinearGains;
    unsigned int _numberOfChannels;
	
};



#endif
