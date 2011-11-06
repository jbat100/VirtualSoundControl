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
#include "VSCSoundMultiChannelElement.h"

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

class VSCSynthSourceElement : public VSCSoundMultiChannelElement {
	
	
public:
    
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
	
};



#endif
