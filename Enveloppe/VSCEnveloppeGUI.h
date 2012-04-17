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

#include "VSCGUI.h"
#include "VSCMath.h"
#include "VSCColour.h"
#include "VSCSound.h"
#include "VSCEnveloppe.h"

#include <boost/shared_ptr.hpp>

/*
 *	VSCEnveloppeViewSetup
 */

class VSCEnveloppeEditorGUIConfig;

typedef boost::shared_ptr<VSCEnveloppeEditorGUIConfig> VSCEnveloppeEditorGUIConfigPtr;

class VSCEnveloppeEditorGUIConfig {
	
public:
	
	VSCEnveloppeEditorGUIConfig(void);
	
	void setToDefault(void);
	
	/*
	 *	View range setters / getters
	 */
    const VSCEnveloppe::TimeRange& getTimeRange(void) const;
	void setTimeRange(const VSCEnveloppe::TimeRange& timeRange);
	const VSCEnveloppe::ValueRange& getValueRange(void) const;
	void setValueRange(const VSCEnveloppe::ValueRange& valueRange);
    
    /*
	 *	Allowed range setters / getters
	 */
    const VSCEnveloppe::TimeRange& getAllowedTimeRange(void) const;
	void setAllowedTimeRange(const VSCEnveloppe::TimeRange& timeRange);
	const VSCEnveloppe::ValueRange& getAllowedValueRange(void) const;
	void setAllowedValueRange(const VSCEnveloppe::ValueRange& valueRange);
    
    /*
	 *	Selection rect setters / getters
	 */
    const VSCColour& getSelectionRectColour(void) const;
    void setSelectionRectColour(const VSCColour& colour);
    VSCSFloat getSelectionRectLineWidth(void) const;
    void setSelectionRectLineWidth(const float width);
	
    /*
	 *	Point selection radius
	 */
    const VSCSFloat getPointSelectionRadius(void) const;
    void setPointSelectionRadius(const VSCSFloat radius);
    
    /*
	 *	Editor size setters / getters
	 */
    const VSC::Size& getEditorSize(void) const;
    void setEditorSize(const VSC::Size& size);
    
    /*
	 *	Point Calculations
	 */
    VSCSFloat pointForTime(const VSCSFloat t);
    VSCSFloat pointForValue(const VSCSFloat v);
    VSCSFloat timeForPoint(const VSCSFloat point);
    VSCSFloat valueForPoint(const VSCSFloat point);
    VSC::Point pointForEnveloppeCoordinate(const VSCEnveloppeCoordinatePtr& p);
    VSCEnveloppeCoordinatePtr enveloppeCoordinateForPoint(const VSC::Point& p);
    
    
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
	 *	Selection rect
	 */
    VSCColour _selectionRectColour;
    VSCSFloat _selectionRectLineWidth;
    
    /*
	 *	Editor Size (used for point/time and point/value conversions)
	 */
    VSC::Size _editorSize;
    
    /*
     *  Point Selection Radius
     */
    VSCSFloat _pointSelectionRadius;

};

/*
 *	VSCEnveloppeGUIConfig
 */

class VSCEnveloppeGUIConfig;

typedef boost::shared_ptr<VSCEnveloppeGUIConfig> VSCEnveloppeGUIConfigPtr;

class VSCEnveloppeGUIConfig {
    
public:
    
    VSCEnveloppeGUIConfig();
    VSCEnveloppeGUIConfig(const VSCEnveloppeGUIConfig& setup);
    
    /*
	 *	Control Points setters / getters
	 */
	float getControlPointRadius(void) const;
	void setControlPointRadius(float controlPointRadius);
	const VSCColour& getControlPointSelectedColour(void) const;
	void setControlPointSelectedColour(const VSCColour& colour);
	const VSCColour& getControlPointUnselectedColour(void) const;
	void setControlPointUnselectedColour(const VSCColour& colour);
	
	/*
	 *	Enveloppe lines setters / getters
	 */
	float getLineWidth(void) const;
	void setLineWidth(float lineWidth);
	const VSCColour& getLineSelectedColour(void) const;
	void setLineSelectedColour(const VSCColour& colour);
    const VSCColour& getLineUnselectedColour(void) const;
	void setLineUnselectedColour(const VSCColour& colour);
    
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
	VSCColour _lineSelectedColour;
    VSCColour _lineUnselectedColour;
    
};

VSCSFloat pointForTime(const VSCSFloat t, const VSCEnveloppe::TimeRange& timeRange, const VSCSFloat width);
VSCSFloat pointForValue(const VSCSFloat v, const VSCEnveloppe::ValueRange& valueRange, const VSCSFloat height);
VSCSFloat timeForPoint(const VSCSFloat point, const VSCEnveloppe::TimeRange& timeRange, const VSCSFloat width);
VSCSFloat valueForPoint(const VSCSFloat point, const VSCEnveloppe::ValueRange& valueRange, const VSCSFloat height);

#endif