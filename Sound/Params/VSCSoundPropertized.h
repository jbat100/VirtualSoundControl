/*
 *  VSCSoundPropertizedElement.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 16/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_SOUND_PROPERTIZED_
#define _VSC_SOUND_PROPERTIZED_

#include "VSCSound.h"
#include "VSCSoundProperties.h"

#include <set>
#include <boost/shared_ptr.hpp>

/*
 *	Smart pointer and iterator convenience defines
 */

#define VSCSoundPropertizedPtr    boost::shared_ptr<VSCSoundPropertized>


/*
 *	Root class for adding/removing setting/getting property values
 */

class VSCSoundPropertized {
	
	
public:
	
	void addPropertyKey(VSCSProperty::Key k);
	void removePropertyKey(VSCSProperty::Key k);
	VSCSProperty::KeySet& getPropertyKeys(void);
	
	virtual std::string getValueForPropertyWithKey(VSCSProperty::Key key);
	virtual void setValueForPropertyWithKey(std::string, VSCSProperty::Key key);
	
private:
	
    VSCSProperty::KeySet _propertyKeys;
	
};

#endif