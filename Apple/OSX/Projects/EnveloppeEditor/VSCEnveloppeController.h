//
//  VSC::EnveloppeController.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/2/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSC::EnveloppeView.h"
#import "VSC::Enveloppe.h"

@protocol VSC::EnveloppeViewDataSource;


@interface VSC::EnveloppeController : NSObject <VSC::EnveloppeViewDataSource> {
    
    VSC::Enveloppe::SPtr _currentEnveloppe;
    VSC::Enveloppe::List _backgroundEnveloppes;
    
}

@property (nonatomic, strong) IBOutlet VSC::EnveloppeView* enveloppeView;
@property (nonatomic, strong) IBOutlet NSSlider* horizontalScaleSlider;

@property (nonatomic, strong) NSString* defaultEnveloppePath;

-(VSC::Enveloppe::SPtr) getCurrentEnveloppe;
-(void) setCurrentEnveloppe:(VSC::Enveloppe::SPtr)enveloppe;

-(void) addEnveloppe:(VSC::Enveloppe::SPtr)enveloppe;
-(void) removeEnveloppe:(VSC::Enveloppe::SPtr)enveloppe;

-(IBAction) loadEnveloppe:(id)sender;
-(IBAction) saveEnveloppe:(id)sender;
-(IBAction) saveEnveloppeAs:(id)sender;

// slider value maps to seconds represented in the enveloppe vue
-(IBAction) scaleSliderValueChanged:(id)sender; 

@end
