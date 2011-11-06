/*
 *  VSCSynthInstrument.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 05/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_SYNTH_INSTRUMENT_
#define _VSC_SYNTH_INSTRUMENT_

#include "VSCSound.h"
#include "VSCSoundMultiChannelElement.h"
#include "VSCSynthSourceGroup.h"

#include <list>
#include <vector>
#include <boost/shared_ptr.hpp>

#define VSCSynthInstrumentPtr		boost::shared_ptr<VSCSynthInstrument>

#define ConstSynthInstrmntIter		std::list<VSCSynthInstrumentPtr>::const_iterator 
#define ConstRevSynthInstrmntIter	std::list<VSCSynthInstrumentPtr>::const_reverse_iterator 

#define SynthSynthInstrmntIter		std::list<VSCSynthInstrumentPtr>::iterator 
#define RevSynthSynthInstrmntIter	std::list<VSCSynthInstrumentPtr>::reverse_iterator 



#ifdef VSCS_USE_STK

#include "Generator.h"

class VSCSynthInstrument : public VSCSoundMultiChannelElement : public stk::Generator {
	
	
public:
    
    VSCSynthInstrument();
    VSCSynthInstrument(unsigned int numberOfChannels);
    
    /*--------------------------------------------------------------*/
    
    /*
     *  METHODS IN THIS SECTION CAN BE CALLED FROM THE CONSTRUCTOR AND SO SHOULD
     *  NOT BE VIRTUAL
     */
	
    /*
     *  Turns off all processing.
     */
	void setOn(bool on);
	bool isOn(void) const;
	
	/*--------------------------------------------------------------*/
	
	/*
	 *	stk::Instrmnt like methods 
	 */
	
	/*
	 *	Note on/off messages
	 */
	// 0.0 < 0 < 127.0    0.0 < amplitude < 127.0
	virtual void noteOn(VSCSFloat note, VSCSFloat amplitude);
	virtual void noteOff(VSCSFloat amplitude); // stk::Instrmnt method (amplitude is used for decay time)
	
	//! Set instrument parameters for a particular frequency.
	virtual void setFrequency( StkFloat frequency );
	
	//! Perform the control change specified by \e number and \e value (0.0 - 128.0).
	virtual void controlChange(int number, StkFloat value);
	
	StkFrames& tick( StkFrames& frames, unsigned int channel = 0 );

	// THIS METHOD IS NOT DECLARED AS VIRTUAL IN INSTRMNT, IT APPARENTLY SHOULD NOT BE SUBCLASSED
	// StkFrames& tick( StkFrames& frames, unsigned int channel = 0 );
	
	// StkFloat tick( void ); // This method is not actually declared in stk::Generator
    
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
	virtual std::string intrumentTypeString(void);
	
	/*--------------------------------------------------------------*/
	
	
protected:
	
	bool _isOn;
	VSCSynthSourceGroup* _sourceGroup;
	
	
};

inline StkFrames& VSCSynthInstrument::tick(StkFrames& frames, unsigned int channel)
{
	assert(_sourceGroup.getNumberOfChannels() == _numberOfChannels);
	
	_sourceGroup.tick(frames, kVSCSAllChannels);
	
	return frames;
}

#endif // VSCS_USE_STK

#endif
