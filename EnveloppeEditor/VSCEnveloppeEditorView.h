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

//#import <CorePlot/CorePlot.h>

//@class CPTGraphHostingView;
//@class CPTXYGraph;

@interface VSCEnveloppeEditorView : NSView { //<CPTPlotDataSource> {
	
    NSSlider* valueZoomSlider;
    NSSlider* timeZoomSlider;
    
    //NSView* graphParentView;
	VSCEnveloppeView* mainEnveloppeView;
    //CPTGraphHostingView* graphHost;
    //CPTXYGraph* graph;

}

@property (nonatomic) IBOutlet NSSlider* valueZoomSlider;
@property (nonatomic) IBOutlet NSSlider* timeZoomSlider;

//@property (nonatomic, retain) IBOutlet NSView* graphParentView;
//@property (nonatomic, retain, readonly) IBOutlet CPTGraphHostingView* graphHost;

@property (nonatomic, readonly) VSCEnveloppeView* mainEnveloppeView;
//@property (nonatomic, retain, readonly) CPTXYGraph* graph;

-(void) setupGraph;
-(void) teardownGraph;
-(void) reajustGraph;

-(void) layoutEnveloppeView;

@end
