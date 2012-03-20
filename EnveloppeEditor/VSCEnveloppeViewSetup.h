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
    const VSCEnveloppe::TimeRange& getTimeRange(void) const;
	void setTimeRange(const VSCEnveloppe::TimeRange timeRange);
	const VSCEnveloppe::ValueRange& getValueRange(void) const;
	void setValueRange(const VSCEnveloppe::ValueRange valueRange);
    
    /*
	 *	Allowed range setters / getters
	 */
    const VSCEnveloppe::TimeRange& getAllowedTimeRange(void) const;
	void setAllowedTimeRange(const VSCEnveloppe::TimeRange timeRange);
	const VSCEnveloppe::ValueRange& getAllowedValueRange(void) const;
	void setAllowedValueRange(const VSCEnveloppe::ValueRange valueRange);
	
	/*
	 *	Display type setters / getters
	 */
	VSCEnveloppeDisplayType getDisplayType(void) const;
	void setDisplayType(const VSCEnveloppeDisplayType displayType);
	
    /*
	 *	Enveloppe display setups
	 */
    VSCEnveloppeDisplaySetup& getActiveDisplaySetup();
	VSCEnveloppeDisplaySetup& getInactiveDisplaySetup();
    
    
    /*
	 *	Enveloppe point calculations
	 */
	
	int pixelForTime(const VSCSFloat t, const int widthInPixels) const;
	int pixelForValue(const VSCSFloat t, const int heightInPixels) const;
	VSCSFloat timeForPixel(const int pixel, const int widthInPixels) const;
	VSCSFloat valueForPixel(const int pixel, const int widthInPixels) const;
	
	
	
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
    
    /*
     *  Enveloppe display setups
     */
    VSCEnveloppeDisplaySetup _activeDisplaySetup;
	VSCEnveloppeDisplaySetup _inactiveDisplaySetup;

};

#endif