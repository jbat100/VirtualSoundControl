//
//  VSCOBOSXElementColorView.m
//  VSCApplication
//
//  Created by Jonathan Thorpe on 3/11/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import "VSCOBOSXElementColorView.h"

#include <boost/assert.hpp>

@interface VSCOBOSXElementColorView ()
{
    @private
    
    id<VSCOBOSXElementController> _elementController;
}

-(void) commonInit;

@end

@implementation VSCOBOSXElementColorView

@dynamic elementController;

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self commonInit];
    }
    return self;
}

-(id) initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self) {
        [self commonInit];
    }
    return self;
}

-(void) commonInit
{

}

- (void)drawRect:(NSRect)dirtyRect
{
    
}

-(void) setupIfNecessary
{
    
    NSMutableArray* viewsForHorizontalConstraints = [NSMutableArray array];
    NSMutableString* verticalVisualFormat = [NSMutableString stringWithString:@"V:|"];
    
    /*
     *  Debug buttons
     */
    
    BOOST_ASSERT(self.elementController);
    
    self.randomizeVertexDiffuseColorsButton = [[NSButton alloc] init];
    self.randomizeVertexDiffuseColorsButton.translatesAutoresizingMaskIntoConstraints = NO;
    [self addSubview:self.randomizeVertexDiffuseColorsButton];
    NSView* randomizeVertexDiffuseColorsButton = self.randomizeVertexDiffuseColorsButton;
    [self.randomizeVertexDiffuseColorsButton setTitle: @"Randomize Diffuse"];
    [self.randomizeVertexDiffuseColorsButton setTarget:self.elementController];
    [self.randomizeVertexDiffuseColorsButton setAction:@selector(randomizeDiffuseVertexColors:)];
    [self.randomizeVertexDiffuseColorsButton setButtonType:NSMomentaryLightButton];
    [self.randomizeVertexDiffuseColorsButton setBezelStyle:NSTexturedSquareBezelStyle];
    [viewsForHorizontalConstraints addObject:randomizeVertexDiffuseColorsButton];
    [verticalVisualFormat appendString:@"-20-[randomizeVertexDiffuseColorsButton(==20)]"];
    
    self.printVertexDiffuseColorsButton = [[NSButton alloc] init];
    self.printVertexDiffuseColorsButton.translatesAutoresizingMaskIntoConstraints = NO;
    [self addSubview:self.printVertexDiffuseColorsButton];
    NSView* printVertexDiffuseColorsButton = self.printVertexDiffuseColorsButton;
    [self.printVertexDiffuseColorsButton setTitle: @"Print Diffuse Colors"];
    [self.printVertexDiffuseColorsButton setTarget:self.elementController];
    [self.printVertexDiffuseColorsButton setAction:@selector(printDiffuseColors:)];
    [self.printVertexDiffuseColorsButton setButtonType:NSMomentaryLightButton];
    [self.printVertexDiffuseColorsButton setBezelStyle:NSTexturedSquareBezelStyle];
    [viewsForHorizontalConstraints addObject:printVertexDiffuseColorsButton];
    [verticalVisualFormat appendString:@"-5-[printVertexDiffuseColorsButton(==20)]"];
    
    self.selectVertexDiffuseColorButton = [[NSButton alloc] init];
    self.selectVertexDiffuseColorButton.translatesAutoresizingMaskIntoConstraints = NO;
    [self addSubview:self.selectVertexDiffuseColorButton];
    NSView* selectVertexDiffuseColorButton = self.selectVertexDiffuseColorButton;
    [self.selectVertexDiffuseColorButton setTitle: @"Vertex Normals"];
    [self.selectVertexDiffuseColorButton setTarget:self.elementController];
    [self.selectVertexDiffuseColorButton setAction:@selector(selectDiffuseColor:)];
    [self.selectVertexDiffuseColorButton setButtonType:NSMomentaryLightButton];
    [self.selectVertexDiffuseColorButton setBezelStyle:NSTexturedSquareBezelStyle];
    [viewsForHorizontalConstraints addObject:selectVertexDiffuseColorButton];
    [verticalVisualFormat appendString:@"-5-[selectVertexDiffuseColorButton(==20)]"];
    
    /*
     *  End and constraint creations
     */
    
    [verticalVisualFormat appendString:@"-(>=10)-|"];
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(randomizeVertexDiffuseColorsButton,
                                                                   printVertexDiffuseColorsButton,
                                                                   selectVertexDiffuseColorButton);
    
    NSLog(@"VerticalVisualFormat: %@", verticalVisualFormat);
    
    [self addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:verticalVisualFormat
                                                                 options:0
                                                                 metrics:nil
                                                                   views:viewsDictionary]];
    
    for (NSView* viewForConstraint in viewsForHorizontalConstraints)
    {
        NSDictionary *localViewsDictionary = NSDictionaryOfVariableBindings(viewForConstraint);
        [self addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-[viewForConstraint]-|"
                                                                     options:0
                                                                     metrics:nil
                                                                       views:localViewsDictionary]];
    }
    
}

-(void) awakeFromNib
{
    
}

#pragma mark - Custom Setters

-(id<VSCOBOSXElementController>)elementController
{
    id<VSCOBOSXElementController> temp;
    @synchronized(self)
    {
        temp = _elementController;
    }
    return temp;
}

-(void)setElementController:(id<VSCOBOSXElementController>)elementController
{
    @synchronized(self)
    {
        _elementController = elementController;
        self.randomizeVertexDiffuseColorsButton.target = _elementController;
        self.printVertexDiffuseColorsButton.target = _elementController;
        self.selectVertexDiffuseColorButton.target = _elementController;
    }
}

@end
