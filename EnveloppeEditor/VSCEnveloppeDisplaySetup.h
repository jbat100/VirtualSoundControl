//
//  VSCEnveloppeDisplaySetup.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 2/6/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_ENVELOPPE_DISPLAY_SETUP_H_
#define _VSC_ENVELOPPE_DISPLAY_SETUP_H_

#include "VSCColour.h"
#include "VSCSound.h"

class VSCEnveloppeDisplaySetup {
    
public:
    
    VSCEnveloppeDisplaySetup();
    VSCEnveloppeDisplaySetup(const VSCEnveloppeDisplaySetup& setup);
    
    /*
	 *	Control Points setters / getters
	 */
	float getControlPointRadius(void) const;
	void setControlPointRadius(float controlPointRadius);
	VSCColour getControlPointSelectedColour(void) const;
	void setControlPointSelectedColour(VSCColour colour);
	VSCColour getControlPointUnselectedColour(void) const;
	void setControlPointUnselectedColour(VSCColour colour);
	
	/*
	 *	Enveloppe lines setters / getters
	 */
	float getLineWidth(void) const;
	void setLineWidth(float lineWidth);
	VSCColour getLineColour(void) const;
	void setLineColour(VSCColour colour);

    
    
private:
    
    /*
	 *	Control Points
	 */
	float _controlPointRadius;
	VSCColour _controlPointSelectedColour;
	VSCColour _controlPointUnselectedColour;
	
	/*
	 *	Enveloppe lines
	 */
	float _lineWidth;
	VSCColour _lineColour;
    
};

#endif
