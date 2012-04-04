//
//  VSCEnveloppeController.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/2/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCEnveloppeView.h"
#import "VSCEnveloppe.h"

@protocol VSCEnveloppeViewDataSource;


@interface VSCEnveloppeController : NSObject <VSCEnveloppeViewDataSource> {
    
    VSCEnveloppePtr _currentEnveloppe;
    
}

@property (nonatomic, strong) IBOutlet VSCEnveloppeView* enveloppeView;
@property (nonatomic, strong) IBOutlet NSSlider* horizontalScaleSlider;

@property (nonatomic, strong) NSString* defaultEnveloppePath;

-(VSCEnveloppePtr) getCurrentEnveloppe;
-(void) setCurrentEnveloppe:(VSCEnveloppePtr)enveloppe;

-(IBAction) loadEnveloppe:(id)sender;
-(IBAction) saveEnveloppe:(id)sender;
-(IBAction) saveEnveloppeAs:(id)sender;


// slider value maps to seconds represented in the enveloppe vue
-(IBAction) scaleSliderValueChanged:(id)sender; 

@end
