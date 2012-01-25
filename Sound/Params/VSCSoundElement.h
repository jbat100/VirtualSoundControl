/*
 *  VSCSoundElement.h
 *  SynthStation
 *
 *  Created by Jonathan Thorpe on 27/11/2011.
 *  Copyright 2011 NXP. All rights reserved.
 *
 */

#ifndef _VSC_SOUND_ELEMENT_
#define _VSC_SOUND_ELEMENT_

#include "VSCSound.h"
#include "VSCSoundPropertized.h"
#include "VSCSoundParameterized.h"

#include <string>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>


/*
 *	Smart pointer and iterator convenience defines
 */

#define VSCSoundElementPtr    boost::shared_ptr<VSCSoundElement>


/*
 *	Root class for all sound elements whether generators, controllers, processors or
 *	multiplexers
 *
 *	boost::enable_shared_from_this multiple inheritance is not supported so we can't
 *	have VSCSoundPropertized and VSCSoundParameterized both inheriting from 
 *	boost::enable_shared_from_this, hence the decision to make VSCSoundElement
 *	inherit from boost::enable_shared_from_this
 */

class VSCSoundElement : public VSCSoundPropertized, public VSCSoundParameterized, 
						public boost::enable_shared_from_this<VSCSoundElement> {
	
public:
    
    friend std::ostream& operator<<(std::ostream& output, const VSCSoundElement& p);
    
    /*
     *  Constructor
     */
	VSCSoundElement();
							
	virtual VSCSParameter::KeyList getInterfaceKeyList(void) const;
	
	/*
	 *	Generator type/identifier (for easier interface, not requiring tons of casts)
	 */
	virtual std::string getElementIdentifier(void) const; 
	virtual std::string getElementType(void) const;
	virtual std::string getValueForPropertyWithKey(VSCSProperty::Key key) const;
							
private:
    
    static const std::string kSoundElementType;
	static unsigned int elementCount;
    unsigned int elementId;
	
};

#endif