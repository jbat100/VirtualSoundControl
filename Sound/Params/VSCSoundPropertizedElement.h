/*
 *  VSCSoundPropertizedElement.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_SOUND_PROPERTIZED_ELEMENT_
#define _VSC_SOUND_PROPERTIZED_ELEMENT_

#include "VSCSound.h"
#include "VSCSoundProperties.h"

#include <set>
#include <boost/shared_ptr.hpp>

/*
 *	Smart pointer and iterator convenience defines
 */

#define VSCSoundPropertizedElementPtr    boost::shared_ptr<VSCSoundPropertizedElement>


/*
 *	Root class for adding/removing setting/getting property values
 */

class VSCSoundPropertizedElement {
	
	
public:
	
	void addPropertyKey(VSCSProperty::Key k);
	void removePropertyKey(VSCSProperty::Key k);
	std::set<VSCSProperty::Key>& getPropertyKeys(void);
	
	virtual double getValueForPropertyWithKey(VSCSProperty::Key key) = 0;
	virtual void setValueForPropertyWithKey(double value, VSCSProperty::Key key) = 0;
	
private:
	
    std::set<VSCSProperty::Key> _propertyKeys;
	
};

#endif