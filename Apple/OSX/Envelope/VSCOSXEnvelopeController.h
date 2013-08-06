//
//  VSCOSXEnvelopeController.h
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 12/16/12.
//
//

#import <Foundation/Foundation.h>

#include "VSCEnvelopeMinimal.h"

typedef enum _VSCOSXEnvelopeViewClickArea {
	VSCOSXEnvelopeViewClickAreaNone,
	VSCOSXEnvelopeViewClickAreaSelection,
	VSCOSXEnvelopeViewClickAreaPoint
} VSCOSXEnvelopeViewClickArea;

/*
 *  Using bitmask type as multiple actions are possible on click down (select, deselect, move, create)
 */

typedef enum _VSCOSXEnvelopeViewMouseAction {
	VSCOSXEnvelopeViewMouseActionNone             = 0,
	VSCOSXEnvelopeViewMouseActionSelect           = 1 << 0,
    VSCOSXEnvelopeViewMouseActionPersistentSelect = 1 << 1,
	VSCOSXEnvelopeViewMouseActionCreate           = 1 << 2,
    VSCOSXEnvelopeViewMouseActionDelete           = 1 << 3,
	VSCOSXEnvelopeViewMouseActionMove             = 1 << 4,
	VSCOSXEnvelopeViewMouseActionCreateControl    = 1 << 5,
	VSCOSXEnvelopeViewMouseActionDeleteControl    = 1 << 6,
    VSCOSXEnvelopeViewMouseActionMoveControl      = 1 << 7,
} VSCOSXEnvelopeViewMouseAction;

@protocol VSCOSXEnvelopeController <NSObject>

-(const VSC::Envelopes&) backgroundEnvelopes;

-(VSC::Envelope_SPtr) currentEnvelope;

@end
