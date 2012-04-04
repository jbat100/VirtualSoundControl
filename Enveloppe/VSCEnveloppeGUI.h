/*
 *  VSCEnveloppeViewSetup.h
 *  EnveloppeEditor
 *
 *  Created by Jonathan Thorpe on 02/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_ENVELOPPE_GUI_H_
#define _VSC_ENVELOPPE_GUI_H_

#include "VSCColour.h"
#include "VSCSound.h"
#include "VSCEnveloppe.h"
#include "VSCEnveloppeDisplaySetup.h"
#include <utility>
#include <list>
#include <boost/shared_ptr.hpp>

/*
 *	Enveloppe point calculations
 */

VSCSFloat pointForTime(const VSCSFloat t, const VSCEnveloppe::TimeRange timeRange, const VSCSFloat width) const;
VSCSFloat pointForValue(const VSCSFloat v, const VSCEnveloppe::ValueRange valueRange, const VSCSFloat height) const;
VSCSFloat timeForPoint(const VSCSFloat point, const VSCEnveloppe::TimeRange timeRange, const VSCSFloat width) const;
VSCSFloat valueForPoint(const VSCSFloat point, const VSCEnveloppe::ValueRange valueRange, const VSCSFloat height) const;

/*
 *	VSCEnveloppeViewSetup
 */

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

/*
 *	VSCEnveloppeDisplaySetup
 */

class VSCEnveloppeDisplaySetup;

typedef boost::shared_ptr<VSCEnveloppeDisplaySetup> VSCEnveloppeDisplaySetupPtr;

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