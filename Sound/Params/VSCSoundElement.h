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

/*
 *	Smart pointer and iterator convenience defines
 */

#define VSCSoundElementPtr    boost::shared_ptr<VSCSoundElement>


/*
 *	Root class for adding/removing setting/getting property values
 */

class VSCSoundElement : public VSCSoundPropertized, public VSCSoundParameterized {
	
public:
    
    /*
     *  Constructor
     */
	VSCSoundElement();
	
	/*
	 *	Generator type/identifier (for easier interface, not requiring tons of casts)
	 */
	virtual std::string getElementIdentifier(void); 
	virtual std::string getElementType(void);
	
private:
    
    static const std::string kSoundElementType;
	
	static unsigned int elementCount;
	
    unsigned int elementId;
	
};

#endif