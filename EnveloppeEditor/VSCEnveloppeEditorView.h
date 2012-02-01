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
#import "CorePlot.h"

@class CPTGraphHostingView;
@class CPTXYGraph;

@interface VSCEnveloppeEditorView : NSView {
	
    NSView* graphParentView;
    
	VSCEnveloppeView* mainEnveloppeView;
    CPTGraphHostingView* graphHost;
    CPTXYGraph* graph;

}

@property (nonatomic, retain) IBOutlet NSView* graphParentView;

@property (nonatomic, retain, readonly) VSCEnveloppeView* mainEnveloppeView;
@property (nonatomic, retain, readonly) CPTGraphHostingView* graphHost;
@property (nonatomic, retain, readonly) CPTXYGraph* graph;

-(void) setupGraph;
-(void) teardownGraph;
-(void) reajustGraph;


@end
