/*
 *  VSCEnveloppeViewSetup.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 02/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_ENVELOPPE_VIEW_SETUP_H_
#define _VSC_ENVELOPPE_VIEW_SETUP_H_

#include "VSCColour.h"
#include "VSCSound.h"
#include "VSCEnveloppe.h"
#include "VSCEnveloppeDisplaySetup.h"
#include <utility>
#include <list>
#include <boost/shared_ptr.hpp>

#define VSCEnveloppeViewSetupPtr    boost::shared_ptr<VSCEnveloppeViewSetup>

typedef enum _VSCEnveloppeDisplayType {
	VSCEnveloppeDisplayTypeDefault = 0,
	VSCEnveloppeDisplayTypeLinear,
	VSCEnveloppeDisplayTypeDB
} VSCEnveloppeDisplayType;


class VSCEnveloppeViewSetup {
	
public:
	
	VSCEnveloppeViewSetup(void);
	
	void setToDefault(void);
	
	/*
	 *	View range setters / getters
	 */
    VSCEnveloppe::TimeRange getTimeRange(void) const;
	void setTimeRange(VSCEnveloppe::TimeRange timeRange);
	VSCEnveloppe::ValueRange getValueRange(void) const;
	void setValueRange(VSCEnveloppe::ValueRange valueRange);
    
    /*
	 *	Allowed range setters / getters
	 */
    VSCEnveloppe::TimeRange getAllowedTimeRange(void) const;
	void setAllowedTimeRange(VSCEnveloppe::TimeRange timeRange);
	VSCEnveloppe::ValueRange getAllowedValueRange(void) const;
	void setAllowedValueRange(VSCEnveloppe::ValueRange valueRange);
	
	/*
	 *	Display type setters / getters
	 */
	VSCEnveloppeDisplayType getDisplayType(void) const;
	void setDisplayType(VSCEnveloppeDisplayType displayType);
	
    
    /*
	 *	Enveloppe point calculations
	 */
	
	int pixelForTime(VSCSFloat t, int widthInPixels) const;
	int pixelForValue(VSCSFloat t, int heightInPixels) const;
	VSCSFloat timeForPixel(int pixel, int widthInPixels) const;
	VSCSFloat valueForPixel(int pixel, int widthInPixels) const;
	
    VSCEnveloppeDisplaySetup& getActiveDisplaySetup();
	VSCEnveloppeDisplaySetup& getInactiveDisplaySetup();
	
	
private:
	
	/*
	 *	View range
	 */
	VSCEnveloppe::TimeRange _timeRange;
	VSCEnveloppe::ValueRange _valueRange;
    
    /*
	 *	Allowed range
	 */
	VSCEnveloppe::TimeRange _allowedTimeRange;
	VSCEnveloppe::ValueRange _allowedValueRange;
	
	/*
	 *	Display type
	 */
	VSCEnveloppeDisplayType _displayType;

};

#endif