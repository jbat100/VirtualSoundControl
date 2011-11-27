/*
 *  VSCSoundParameterizedElement.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */


#ifndef _VSC_SOUND_PARAMETERIZED_
#define _VSC_SOUND_PARAMETERIZED_

#include "VSCSound.h"
#include "VSCSoundParameters.h"

#include <set>
#include <boost/shared_ptr.hpp>

/*
 *	Smart pointer and iterator convenience defines
 */

#define VSCSoundParameterizedPtr    boost::shared_ptr<VSCSoundParameterized>


/*
 *	Root class for adding/removing setting/getting parameter values
 */

class VSCSoundParameterized {
	
	
public:
	
	void addParameterKey(VSCSParameter::Key k);
	void removeParameterKey(VSCSParameter::Key k);
	std::set<VSCSParameter::Key>& getParameterKeys(void);
	
	virtual double getValueForParameterWithKey(VSCSParameter::Key key);
	virtual void setValueForParameterWithKey(double value, VSCSParameter::Key key);
	
protected:
	
    std::set<VSCSParameter::Key> _parameterKeys;
	
};




#endif