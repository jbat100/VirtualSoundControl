/*
 *  VSCOSXEnvelopeViewSetup.h
 *  EnvelopeEditor
 *
 *  Created by Jonathan Thorpe on 02/08/2011.
 *  Copyright 2011 JBAT. All rights reserved.
 *
 */

#ifndef _VSC_ENVELOPE_GUI_H_
#define _VSC_ENVELOPE_GUI_H_

#include "VSCGUI.h"
#include "VSCMath.h"
#include "VSCColor.h"
#include "VSCSound.h"
#include "VSCEnvelope.h"

#include <boost/shared_ptr.hpp>

namespace VSC {
    

    class EnvelopeEditorGUIConfig {
        
    public:
        
        EnvelopeEditorGUIConfig(void);
        
        void setToDefault(void);
        
        /*
         *	View range setters / getters
         */
        const Envelope::TimeRange& getTimeRange(void) const;
        void setTimeRange(const Envelope::TimeRange& timeRange);
        const Envelope::ValueRange& getValueRange(void) const;
        void setValueRange(const Envelope::ValueRange& valueRange);
        
        /*
         *	Allowed range setters / getters
         */
        const Envelope::TimeRange& getAllowedTimeRange(void) const;
        void setAllowedTimeRange(const Envelope::TimeRange& timeRange);
        const Envelope::ValueRange& getAllowedValueRange(void) const;
        void setAllowedValueRange(const Envelope::ValueRange& valueRange);
        
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
        Point pointForEnvelopeCoordinate(const EnvelopeCoordinate::SPtr& p);
        EnvelopeCoordinate::SPtr createEnvelopeCoordinateForPoint(const Point& p);
        void setEnvelopeCoordinateToPoint(EnvelopeCoordinate::SPtr coord, const Point& p);
        
        
    private:
        
        /*
         *	View range
         */
        Envelope::TimeRange _timeRange;
        Envelope::ValueRange _valueRange;
        
        /*
         *	Allowed range
         */
        Envelope::TimeRange mAllowedTimeRange;
        Envelope::ValueRange mAllowedValueRange;
        
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


    class EnvelopeGUIConfig {
        
    public:
        
        EnvelopeGUIConfig();
        EnvelopeGUIConfig(const EnvelopeGUIConfig& setup);
        
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
         *	Envelope lines setters / getters
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
         *	Envelope lines
         */
        Float _lineWidth;
        Color _lineSelectedColour;
        Color _lineUnselectedColour;
        
    };
    
}

#endif