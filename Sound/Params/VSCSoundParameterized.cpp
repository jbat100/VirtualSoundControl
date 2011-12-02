/*
 *  VSCSoundParameterizedElement.cpp
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#include "VSCSoundParameterized.h"
#include "VSCException.h"

const VSCSParameter::KeySet& VSCSoundParameterized::getKeySet(void) {
    return _keySet;
}


void VSCSoundParameterized::addParameterKey(VSCSParameter::Key k) {
	_keySet.insert(k);	
}

void VSCSoundParameterized::removeParameterKey(VSCSParameter::Key k) {
    _keySet.erase(k);	
}

/*
 *	If this class gets these messages then all subclasses have failed to deal with it
 *	so throw...
 */

double VSCSoundParameterized::getValueForParameterWithKey(VSCSParameter::Key key) {
    
    /*
    VSCSParameter::KeyValueMap::iterator iter = _keyValueMap.find(key);
    
    if (iter != _keyValueMap.end()) {
        return (*iter).second;
    }
    */
    
    throw VSCSBadParameterException();
    
}

void VSCSoundParameterized::setValueForParameterWithKey(double value, VSCSParameter::Key key) {
    
    /*
    VSCSParameter::KeyValueMap::iterator iter = _keyValueMap.find(key);
    
    if (iter != _keyValueMap.end()) {
        (*iter).second = value;
    }
     &*/
    
	throw VSCSBadParameterException();

}


