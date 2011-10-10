//
//  VSCEnveloppeEditorView.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 10/10/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

#import "VSCEnveloppeView.h"
#import "VSCEnveloppeViewSetup.h"

@interface VSCEnveloppeEditorView : NSView {
	
	VSCEnveloppeView* mainEnveloppeView;
	
	/*
     *  A view setup
     */
    VSCEnveloppeViewSetupPtr _enveloppeViewSetup;

}

@property (nonatomic, retain) IBOutlet VSCEnveloppeView* mainEnveloppeView;

/* 
 *  Basic C++ setters / getters *
 */
-(VSCEnveloppeViewSetupPtr) getEnveloppeViewSetup;
-(void)setEnveloppeViewSetup:(VSCEnveloppeViewSetupPtr)enveloppe; 

@end
