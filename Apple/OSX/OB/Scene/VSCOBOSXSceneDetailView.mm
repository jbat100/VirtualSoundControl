//
//  VSCOBOSXSceneDetailView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/17/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOBOSXSceneDetailView.h"
#import "VSCOBOSXSceneController.h"

#import "VSCOSXInterfaceFactory.h"
#import "VSCOSXKeyedCheckBoxView.h"
#import "VSCOSXKeyedSliderView.h"

#include "VSCOBScene.h"

@interface VSCOBOSXSceneDetailView ()


@end

@implementation VSCOBOSXSceneDetailView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
    }
    
    return self;
}

- (void)drawRect:(NSRect)dirtyRect
{
    //CGContextRef myContext = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    //CGContextSetGrayFillColor (myContext, 0.3, 1.0);
    //CGContextFillRect(myContext, NSRectToCGRect(self.bounds));
}

- (BOOL) isFlipped
{
    return YES;
} 

-(void) awakeFromNib
{
    /*
     *  Perform setup
     */
    
    BOOST_ASSERT(self.sceneController);
    
    VSCOSXInterfaceFactory* factory = [VSCOSXInterfaceFactory defaultFactory];
    
    NSMutableArray* viewsForHorizontalConstraints = [NSMutableArray array];
    NSMutableString* verticalVisualFormat = [NSMutableString stringWithString:@"V:|"];
    
    /*
     *  Check Boxes
     */
    
    self.pausePhysicsCheckBoxView = [factory newKeyedCheckBoxViewWithOwner:self.sceneController];
    [self addSubview:self.pausePhysicsCheckBoxView];
    [self.pausePhysicsCheckBoxView.checkBoxButton bind:@"value" toObject:self.sceneController withKeyPath:@"pausePhysics" options:nil];
    [self.pausePhysicsCheckBoxView.checkBoxButton setTitle:@"Pause Physics"];
    NSView* pausePhysicsCB = self.pausePhysicsCheckBoxView;
    [viewsForHorizontalConstraints addObject:pausePhysicsCB];
    [verticalVisualFormat appendString:@"-[pausePhysicsCB]"];
    
    self.drawAABBCheckBoxView = [factory newKeyedCheckBoxViewWithOwner:self.sceneController];
    [self addSubview:self.drawAABBCheckBoxView];
    [self.drawAABBCheckBoxView.checkBoxButton bind:@"value" toObject:self.sceneController withKeyPath:@"drawAABB" options:nil];
    [self.drawAABBCheckBoxView.checkBoxButton setTitle:@"Draw AABB"];
    NSView* drawAABBCB = self.drawAABBCheckBoxView;
    [viewsForHorizontalConstraints addObject:drawAABBCB];
    [verticalVisualFormat appendString:@"-0-[drawAABBCB]"];
    
    self.drawWireFrameCheckBoxView = [factory newKeyedCheckBoxViewWithOwner:self.sceneController];
    [self addSubview:self.drawWireFrameCheckBoxView];
    [self.drawWireFrameCheckBoxView.checkBoxButton bind:@"value" toObject:self.sceneController withKeyPath:@"drawWireFrame" options:nil];
    [self.drawWireFrameCheckBoxView.checkBoxButton setTitle:@"Draw WireFrame"];
    NSView* drawWireFrameCB = self.drawWireFrameCheckBoxView;
    [viewsForHorizontalConstraints addObject:drawWireFrameCB];
    [verticalVisualFormat appendString:@"-0-[drawWireFrameCB]"];
    
    /*
     *  Sliders
     */
    
    self.shootSpeedSliderView = [factory newKeyedSliderViewWithOwner:self.sceneController];
    [self addSubview:self.shootSpeedSliderView];
    [self.shootSpeedSliderView.slider bind:@"value" toObject:self.sceneController withKeyPath:@"shootSpeed" options:nil];
    [self.shootSpeedSliderView.valueTextField bind:@"value" toObject:self.sceneController withKeyPath:@"shootSpeed" options:nil];
    [self.shootSpeedSliderView.labelTextField setStringValue:@"Shoot Speed"];
    self.shootSpeedSliderView.slider.maxValue = 100.0;
    self.shootSpeedSliderView.slider.minValue = 0.0;
    NSView* shootSpeedSV = self.shootSpeedSliderView;
    [viewsForHorizontalConstraints addObject:shootSpeedSV];
    [verticalVisualFormat appendString:@"-[shootSpeedSV]"];
    
    self.shootSizeSliderView = [factory newKeyedSliderViewWithOwner:self.sceneController];
    [self addSubview:self.shootSizeSliderView];
    [self.shootSizeSliderView.slider bind:@"value" toObject:self.sceneController withKeyPath:@"shootSize" options:nil];
    [self.shootSizeSliderView.valueTextField bind:@"value" toObject:self.sceneController withKeyPath:@"shootSize" options:nil];
    [self.shootSizeSliderView.labelTextField setStringValue:@"Shoot Size"];
    self.shootSizeSliderView.slider.maxValue = 10.0;
    self.shootSizeSliderView.slider.minValue = 0.0;
    NSView* shootSizeSV = self.shootSizeSliderView;
    [viewsForHorizontalConstraints addObject:shootSizeSV];
    [verticalVisualFormat appendString:@"-2-[shootSizeSV]"];
    
    self.cameraSpeedSliderView = [factory newKeyedSliderViewWithOwner:self.sceneController];
    [self addSubview:self.cameraSpeedSliderView];
    [self.cameraSpeedSliderView.slider bind:@"value" toObject:self.sceneController withKeyPath:@"cameraSpeed" options:nil];
    [self.cameraSpeedSliderView.valueTextField bind:@"value" toObject:self.sceneController withKeyPath:@"cameraSpeed" options:nil];
    [self.cameraSpeedSliderView.labelTextField setStringValue:@"Camera Speed"];
    self.cameraSpeedSliderView.slider.maxValue = 100.0;
    self.cameraSpeedSliderView.slider.minValue = 3.0;
    NSView* cameraSpeedSV = self.cameraSpeedSliderView;
    [viewsForHorizontalConstraints addObject:cameraSpeedSV];
    [verticalVisualFormat appendString:@"-2-[cameraSpeedSV]"];
    
    [verticalVisualFormat appendString:@"-|"];
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(pausePhysicsCB, drawAABBCB, drawWireFrameCB, shootSpeedSV, cameraSpeedSV,
                                                                   shootSizeSV);
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


@end
