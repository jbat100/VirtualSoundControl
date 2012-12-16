//
//  VSCOSXEnvelopeViewController.h
//  EnvelopeEditor
//
//  Created by Jonathan Thorpe on 12/16/12.
//
//

#import <Cocoa/Cocoa.h>

#include "VSCEnvelope.h"

@interface VSCOSXEnvelopeViewController : NSViewController

@property (nonatomic, strong) IBOutlet VSCEnvelopeView* envelopeView;

@property (nonatomic, weak) IBOutlet NSSlider* valueZoomSlider;
@property (nonatomic, weak) IBOutlet NSSlider* timeZoomSlider;

@property (nonatomic, copy) NSString* defaultEnvelopePath;

@property (nonatomic, assign) VSC::Envelope::WPtr envelope;

-(IBAction) loadEnvelope:(id)sender;
-(IBAction) saveEnvelope:(id)sender;
-(IBAction) saveEnvelopeAs:(id)sender;

-(IBAction) scaleSliderValueChanged:(id)sender;

@end
