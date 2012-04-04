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

class VSCEnveloppeViewSetup;

typedef boost::shared_ptr<VSCEnveloppeViewSetup> VSCEnveloppeViewSetupPtr;


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
	 *	Enveloppe point calculations
	 */
	
	float pointForTime(const VSCSFloat t, const float width) const;
	float pointForValue(const VSCSFloat t, const float height) const;
	VSCSFloat timeForPoint(const int point, const int width) const;
	VSCSFloat valueForPoint(const int point, const int widthInPixels) const;
	
	
	
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
	
    

};

#endif