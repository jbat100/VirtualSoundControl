//
//  VSCSynthSourceElement.h
//  SynthStation
//
//  Created by Jonathan Thorpe on 9/26/11.
//  Copyright 2011 VSC. All rights reserved.
//

#ifndef _VSC_SYNTH_SOURCE_ELEMENT_
#define _VSC_SYNTH_SOURCE_ELEMENT_

class VSCSynthSourceElement {
	
public:
	
	/* value between 0 and 1 */
	void setLinearGain(double g);
	double getLinearGet(void);
	
	/* value between 0 and minus infinity */
	void setDBGain(double g);	
	double getDBGain(void);
	
	void setOn(bool on);
	bool isOn(void);
	
	
protected:
	
	bool _isOn;
	double _linearGain; /* value between 0 and 1 */
	
};



#endif
