//
//  VSCOSXEnvelopeViewController.h
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 12/16/12.
//
//

#import <Cocoa/Cocoa.h>

#include "VSCEnvelopeMinimal.h"

@class VSCOSXEnvelopeView;


@interface VSCOSXEnvelopeViewController : NSViewController

@property (nonatomic, strong) IBOutlet VSCOSXEnvelopeView* envelopeView;

@property (nonatomic, weak) IBOutlet NSSlider* valueZoomSlider;
@property (nonatomic, weak) IBOutlet NSSlider* timeZoomSlider;

@property (nonatomic, copy) NSString* defaultEnvelopePath;

@property (nonatomic, assign) VSC::Envelope_SPtr currentEnvelope;

-(void) addBackgroundEnvelope:(VSC::Envelope_SPtr)envelope;
-(void) removeBackgroundEnvelope:(VSC::Envelope_SPtr)envelope;
-(void) clearBackgroundEnvelopes;
-(const VSC::Envelopes&) backgroundEnvelopes;

-(IBAction) autoZoom:(id)sender;
-(IBAction) loadEnvelope:(id)sender;
-(IBAction) saveEnvelope:(id)sender;
-(IBAction) saveEnvelopeAs:(id)sender;
-(IBAction) scaleSliderValueChanged:(id)sender;


@end
