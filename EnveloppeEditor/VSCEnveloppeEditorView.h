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
#import "VSCSound.h"

@interface VSCEnveloppeEditorView : NSView {
	
	VSCEnveloppeView* mainEnveloppeView;

}

@property (nonatomic, retain) IBOutlet VSCEnveloppeView* mainEnveloppeView;


@end
