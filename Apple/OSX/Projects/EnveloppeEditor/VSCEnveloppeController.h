//
//  VSC::EnvelopeController.h
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 4/2/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSC::EnvelopeView.h"
#import "VSC::Envelope.h"

@protocol VSC::EnvelopeViewDataSource;


@interface VSC::EnvelopeController : NSObject <VSC::EnvelopeViewDataSource> {
    
    VSC::Envelope::SPtr _currentEnvelope;
    VSC::Envelope::List _backgroundEnvelopes;
    
}

@property (nonatomic, strong) IBOutlet VSC::EnvelopeView* envelopeView;
@property (nonatomic, strong) IBOutlet NSSlider* horizontalScaleSlider;

@property (nonatomic, strong) NSString* defaultEnvelopePath;

-(VSC::Envelope::SPtr) getCurrentEnvelope;
-(void) setCurrentEnvelope:(VSC::Envelope::SPtr)envelope;

-(void) addEnvelope:(VSC::Envelope::SPtr)envelope;
-(void) removeEnvelope:(VSC::Envelope::SPtr)envelope;

-(IBAction) loadEnvelope:(id)sender;
-(IBAction) saveEnvelope:(id)sender;
-(IBAction) saveEnvelopeAs:(id)sender;

// slider value maps to seconds represented in the envelope vue
-(IBAction) scaleSliderValueChanged:(id)sender; 

@end
