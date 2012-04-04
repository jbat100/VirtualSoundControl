//
//  VSCEnveloppeEditorView.mm
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 10/10/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "VSCEnveloppeEditorView.h"

@interface VSCEnveloppeEditorView ()

//@property (nonatomic, retain, readwrite) CPTGraphHostingView* graphHost;
//@property (nonatomic, retain, readwrite) CPTXYGraph* graph;
@property (nonatomic, readwrite) VSCEnveloppeView* mainEnveloppeView;

-(NSRect) requiredEnveloppeViewFrame;

@end


@implementation VSCEnveloppeEditorView

@synthesize mainEnveloppeView;

//@synthesize graph, graphHost;
// @synthesize graphParentView;

@synthesize valueZoomSlider, timeZoomSlider;

- (id)initWithFrame:(NSRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    return self;
}

-(id) initWithCoder:(NSCoder *)aDecoder {
    self = [super initWithCoder:aDecoder];
    if (self) {
        // Initialization code here.
    }
    return self;
}

-(void) dealloc {
    
    [self teardownGraph];
    
}

-(void)setupGraph 
{    
    
    /*
    
    NSAssert(self.graphHost != nil, @"Expected graphHost");
    
    //NSAssert(self.mainEnveloppeView, @"Expected mainEnveloppeView");
    NSAssert(self.graph == nil, @"Expected nil graph");
    
    NSRect f = self.graphHost.frame;
    f.origin.x = f.origin.y = 0;
    
	self.graph = [[[CPTXYGraph alloc] initWithFrame:f] autorelease];
	CPTTheme *theme = [CPTTheme themeNamed:kCPTStocksTheme];
	[graph applyTheme:theme];
    
	//graph.frame = mainEnveloppeView.bounds;
    
	graph.paddingRight = 50.0f;
    graph.paddingLeft = 50.0f;
    graph.plotAreaFrame.masksToBorder = NO;
    graph.plotAreaFrame.cornerRadius = 0.0f;
    
    CPTMutableLineStyle *borderLineStyle = [CPTMutableLineStyle lineStyle];
    borderLineStyle.lineColor = [CPTColor whiteColor];
    borderLineStyle.lineWidth = 2.0f;
    graph.plotAreaFrame.borderLineStyle = borderLineStyle;
	self.graphHost.hostedGraph = graph;
    
    // Axes
    CPTXYAxisSet *xyAxisSet = (id)graph.axisSet;
    CPTXYAxis *xAxis = xyAxisSet.xAxis;
    CPTMutableLineStyle *lineStyle = [xAxis.axisLineStyle mutableCopy];
    lineStyle.lineCap = kCGLineCapButt;
    xAxis.axisLineStyle = lineStyle;
	[lineStyle release];
    xAxis.labelingPolicy = CPTAxisLabelingPolicyNone;
    
    CPTXYAxis *yAxis = xyAxisSet.yAxis;
    yAxis.axisLineStyle = nil;
    
    // Line plot with gradient fill
	CPTScatterPlot *dataSourceLinePlot = [[[CPTScatterPlot alloc] initWithFrame:graph.bounds] autorelease];
    dataSourceLinePlot.identifier = @"Data Source Plot";
	dataSourceLinePlot.dataLineStyle = nil;
    dataSourceLinePlot.dataSource = self;
	[graph addPlot:dataSourceLinePlot];
	
	CPTColor *areaColor = [CPTColor colorWithComponentRed:1.0 green:1.0 blue:1.0 alpha:0.6];
    CPTGradient *areaGradient = [CPTGradient gradientWithBeginningColor:areaColor endingColor:[CPTColor clearColor]];
    areaGradient.angle = -90.0f;
	CPTFill *areaGradientFill = [CPTFill fillWithGradient:areaGradient];
    dataSourceLinePlot.areaFill = areaGradientFill;
    dataSourceLinePlot.areaBaseValue = CPTDecimalFromDouble(200.0);
    
	areaColor = [CPTColor colorWithComponentRed:0.0 green:1.0 blue:0.0 alpha:0.6];
    areaGradient = [CPTGradient gradientWithBeginningColor:[CPTColor clearColor] endingColor:areaColor];
    areaGradient.angle = -90.0f;
	areaGradientFill = [CPTFill fillWithGradient:areaGradient];
    dataSourceLinePlot.areaFill2 = areaGradientFill;
    dataSourceLinePlot.areaBaseValue2 = CPTDecimalFromDouble(400.0);
     
     */
    
    /*
    // OHLC plot
    CPTMutableLineStyle *whiteLineStyle = [CPTMutableLineStyle lineStyle];
    whiteLineStyle.lineColor = [CPTColor whiteColor];
    whiteLineStyle.lineWidth = 1.0f;
    CPTTradingRangePlot *ohlcPlot = [[[CPTTradingRangePlot alloc] initWithFrame:graph.bounds] autorelease];
    ohlcPlot.identifier = @"OHLC";
    ohlcPlot.lineStyle = whiteLineStyle;
	CPTMutableTextStyle *whiteTextStyle = [CPTMutableTextStyle textStyle];
    whiteTextStyle.color = [CPTColor whiteColor];
	whiteTextStyle.fontSize = 8.0;
	ohlcPlot.labelTextStyle = whiteTextStyle;
	ohlcPlot.labelOffset = 5.0;
    ohlcPlot.stickLength = 2.0f;
    ohlcPlot.dataSource = self;
    ohlcPlot.plotStyle = CPTTradingRangePlotStyleOHLC;
    [graph addPlot:ohlcPlot];
     */
    
    
    /*
    
	// Add plot space for horizontal bar charts
    CPTXYPlotSpace *volumePlotSpace = [[CPTXYPlotSpace alloc] init];
	volumePlotSpace.identifier = @"Volume Plot Space";
    [graph addPlotSpace:volumePlotSpace];
    [volumePlotSpace release];
	
	CPTBarPlot *volumePlot = [CPTBarPlot tubularBarPlotWithColor:[CPTColor blackColor] horizontalBars:NO];
    volumePlot.dataSource = self;
    
    lineStyle = [volumePlot.lineStyle mutableCopy];
    lineStyle.lineColor = [CPTColor whiteColor];
    volumePlot.lineStyle = lineStyle;
    [lineStyle release];
    
    volumePlot.fill = nil; 
	volumePlot.barWidth = CPTDecimalFromFloat(1.0f);
    volumePlot.identifier = @"Volume Plot";
    [graph addPlot:volumePlot toPlotSpace:volumePlotSpace];
    
    [graph reloadData];
    
    self.mainEnveloppeView = [[VSCEnveloppeView alloc] initWithFrame:[self requiredEnveloppeViewFrame]];
    [graphHost addSubview:mainEnveloppeView];
    [graphHost setNeedsDisplay:YES];
    [mainEnveloppeView setNeedsDisplay:YES];
    
    CALayer *viewLayer = [CALayer layer];
    
    [viewLayer setBackgroundColor:CGColorCreateGenericRGB(0.0, 0.0, 0.0, 0.0)]; //RGB plus Alpha Channel
    [viewLayer setDelegate:mainEnveloppeView];
     
     */
    
    /*
     * We don't want to call [self setWantsLayer:YES]; BEFORE setLayer as we want a layer hosting view, not a layer backed view
     
     APPLE DOC:
     
     A layer-backed view is a view that is backed by a Core Animation layer. 
     Any drawing done by the view is cached in the backing layer. You configure a layer-backed view by invoking setWantsLayer: 
     with a value of YES. The view class automatically creates a backing layer for you (using makeBackingLayer if overridden), and 
     you must use the view class’s drawing mechanisms. When using layer-backed views you should never interact directly with the layer. 
     Instead you must use the standard view programming practices.
     
     */
    
    //[self setWantsLayer:YES]; // view's backing store is using a Core Animation Layer
    
    //[mainEnveloppeView setLayer:viewLayer];
    
    /*
     * We DO want to call [self setWantsLayer:YES]; AFTER setLayer
     *
     
     APPLE DOC:
     
     A layer-hosting view is a view that contains a Core Animation layer that you intend to manipulate directly. 
     You create a layer-hosting view by instantiating a Core Animation layer class and supplying that layer to the 
     view’s setLayer: method. After doing so, you then invoke setWantsLayer: with a value of YES. This method order 
     is crucial. When using a layer-hosting view you should not rely on the view for drawing, nor should you add 
     subviews to the layer-hosting view. The root layer (the layer set using setLayer:) should be treated as the 
     root layer of the layer tree and you should only use Core Animation drawing and animation methods. You still 
     use the view for handling mouse and keyboard events, but any resulting drawing must be handled by Core Animation.
     
     */
    
    //[mainEnveloppeView setWantsLayer:YES];
	
    //[viewLayer setNeedsDisplay];
    
}

-(void) teardownGraph {
    
}

-(void) reajustGraph {
    
}

/*
-(NSRect) requiredEnveloppeViewFrame {
    return graph.plotAreaFrame.frame;
}

- (void)resizeSubviewsWithOldSize:(NSSize)oldBoundsSize {
    [super resizeSubviewsWithOldSize:oldBoundsSize];
    self.mainEnveloppeView.frame = [self requiredEnveloppeViewFrame];
}

-(void) layoutEnveloppeView {
    self.mainEnveloppeView.frame = [self requiredEnveloppeViewFrame];
    [self.mainEnveloppeView redrawEnveloppeLayer];
}
 
  */
 

#pragma mark - Plot Data Source Methods

/*

-(NSUInteger)numberOfRecordsForPlot:(CPTPlot *)plot {
    return 10;
}

-(NSNumber *)numberForPlot:(CPTPlot *)plot field:(NSUInteger)fieldEnum recordIndex:(NSUInteger)index {
    NSDecimalNumber *num = [NSDecimalNumber zero];
    return num;
}

-(CPTLayer *)dataLabelForPlot:(CPTPlot *)plot recordIndex:(NSUInteger)index 
{
	if ( ![(NSString *)plot.identifier isEqualToString:@"OHLC"] )
		return (id)[NSNull null]; // Don't show any label
    else if ( index % 5 ) {
        return (id)[NSNull null];
	}
	else {
		return nil; // Use default label style
	}
}
 
 */


@end
