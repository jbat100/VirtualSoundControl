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
    
    /*  Decided against maintaining a key map for performance (parameter set can occur on every single audio sample)
     *  so traversing the entire class hierachy to get to this class seems a poor design choice, also some parameters
     *  for example KeyChannelx and KeyChannelDBx control the same things and maintaining internal consistency would add
     *  another layer of complexity on every cycle, subclasses should therefore store their own state in private variables
     *  perhaps even protected as in the case of multichannel, surrender a bit of encapsulation for better performance
     *  seems justified.
     
    const VSCSParameter::KeyValueMap& getKeyValueMap(void);
	
     */
    
    const VSCSParameter::KeySet& getKeySet(void); // const handle to internals, bad me
    
    /*
     *  Make the setter virtual so that subclasses can 
     */
	virtual double getValueForParameterWithKey(VSCSParameter::Key key);
    virtual void setValueForParameterWithKey(double value, VSCSParameter::Key key);
	
private:
	
    /*  Not used (see accessor comments above)
    VSCSParameter::KeyValueMap _keyValueMap;
	*/
    
    /*
     *  Maintain a key set of the parameters the object can handle (fill in constructors)
     */
    VSCSParameter::KeySet _keySet;
};




#endif