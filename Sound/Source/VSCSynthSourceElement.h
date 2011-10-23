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

class VSCSynthSourceElement {
	
public:
	
	/* value between 0 and 1 */
	void setLinearGain(VSCSFloat g);
	VSCSFloat getLinearGet(void);
	
	/* value between 0 and minus infinity */
	void setDBGain(VSCSFloat g);	
	VSCSFloat getDBGain(void);
	
	void setOn(bool on);
	bool isOn(void);
	
	
protected:
	
	bool _isOn;
	VSCSFloat _linearGain; /* value between 0 and 1 */
	
};



#endif
