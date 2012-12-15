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
#include "VSCColor.h"
#include "VSCSound.h"
#include "VSCEnveloppe.h"

#include <boost/shared_ptr.hpp>

namespace VSC {
    

    class EnveloppeEditorGUIConfig {
        
    public:
        
        EnveloppeEditorGUIConfig(void);
        
        void setToDefault(void);
        
        /*
         *	View range setters / getters
         */
        const Enveloppe::TimeRange& getTimeRange(void) const;
        void setTimeRange(const Enveloppe::TimeRange& timeRange);
        const Enveloppe::ValueRange& getValueRange(void) const;
        void setValueRange(const Enveloppe::ValueRange& valueRange);
        
        /*
         *	Allowed range setters / getters
         */
        const Enveloppe::TimeRange& getAllowedTimeRange(void) const;
        void setAllowedTimeRange(const Enveloppe::TimeRange& timeRange);
        const Enveloppe::ValueRange& getAllowedValueRange(void) const;
        void setAllowedValueRange(const Enveloppe::ValueRange& valueRange);
        
        /*
         *	Selection rect setters / getters
         */
        const Color& getSelectionRectColour(void) const;
        void setSelectionRectColour(const Color& colour);
        Float getSelectionRectLineWidth(void) const;
        void setSelectionRectLineWidth(const Float width);
        
        /*
         *	Point selection radius
         */
        Float getPointSelectionRadius(void) const;
        void setPointSelectionRadius(const Float radius);
        
        /*
         *	Editor size setters / getters
         */
        const Size& getEditorSize(void) const;
        void setEditorSize(const Size& size);
        
        /*
         *	Point Calculations
         */
        Float pointForTime(const Float t);
        Float pointForValue(const Float v);
        Float timeForPoint(const Float point);
        Float valueForPoint(const Float point);
        Float timeDeltaForPointDelta(const Float delta);
        Float valueDeltaForPointDelta(const Float delta);
        Point pointForEnveloppeCoordinate(const EnveloppeCoordinate::SPtr& p);
        EnveloppeCoordinate::SPtr createEnveloppeCoordinateForPoint(const Point& p);
        void setEnveloppeCoordinateToPoint(EnveloppeCoordinate::SPtr coord, const Point& p);
        
        
    private:
        
        /*
         *	View range
         */
        Enveloppe::TimeRange _timeRange;
        Enveloppe::ValueRange _valueRange;
        
        /*
         *	Allowed range
         */
        Enveloppe::TimeRange _allowedTimeRange;
        Enveloppe::ValueRange _allowedValueRange;
        
        /*
         *	Selection rect
         */
        Color _selectionRectColour;
        Float _selectionRectLineWidth;
        
        /*
         *	Editor Size (used for point/time and point/value conversions)
         */
        Size _editorSize;
        
        /*
         *  Point Selection Radius
         */
        Float _pointSelectionRadius;

    };


    class EnveloppeGUIConfig {
        
    public:
        
        EnveloppeGUIConfig();
        EnveloppeGUIConfig(const EnveloppeGUIConfig& setup);
        
        /*
         *	Control Points setters / getters
         */
        Float getControlPointRadius(void) const;
        void setControlPointRadius(Float controlPointRadius);
        const Color& getControlPointSelectedColour(void) const;
        void setControlPointSelectedColour(const Color& colour);
        const Color& getControlPointUnselectedColour(void) const;
        void setControlPointUnselectedColour(const Color& colour);
        
        /*
         *	Enveloppe lines setters / getters
         */
        Float getLineWidth(void) const;
        void setLineWidth(Float lineWidth);
        const Color& getLineSelectedColour(void) const;
        void setLineSelectedColour(const Color& colour);
        const Color& getLineUnselectedColour(void) const;
        void setLineUnselectedColour(const Color& colour);
        
    private:
        
        /*
         *	Control Points
         */
        Float _controlPointRadius;
        Color _controlPointSelectedColour;
        Color _controlPointUnselectedColour;
        
        /*
         *	Enveloppe lines
         */
        Float _lineWidth;
        Color _lineSelectedColour;
        Color _lineUnselectedColour;
        
    };
    
}

#endif