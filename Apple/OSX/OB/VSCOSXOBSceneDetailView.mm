//
//  VSCOSXOBSceneDetailView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/17/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneDetailView.h"
#import "VSCOSXOBSceneController.h"

#import "VSCOSXInterfaceFactory.h"
#import "VSCOSXKeyedCheckBoxView.h"
#import "VSCOSXKeyedSliderView.h"

#include "VSCOBScene.h"

@interface VSCOSXOBSceneDetailView ()

-(NSInteger) stateForSceneSetting:(VSC::OB::Scene::Setting)setting;
-(VSC::OB::Scene::Setting) settingForCheckBox:(id)checkBox;

@property (nonatomic, strong) NSArray* allCheckBoxViews;

@end

@implementation VSCOSXOBSceneDetailView

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
    CGContextRef myContext = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    CGContextSetGrayFillColor (myContext, 0.8, 1.0);
    CGContextFillRect(myContext, NSRectToCGRect(self.bounds));
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
    
    self.pausePhysicsCheckBoxView = [factory newKeyedCheckBoxViewWithOwner:self.sceneController];
    [self.pausePhysicsCheckBoxView.checkBoxButton bind:@"value" toObject:self.sceneController withKeyPath:@"pausePhysics" options:nil];
    [self.pausePhysicsCheckBoxView.labelTextField setStringValue:@"Pause Physics"];
    NSView* pausePhysicsCB = self.pausePhysicsCheckBoxView;
    [viewsForHorizontalConstraints addObject:pausePhysicsCB];
    [verticalVisualFormat appendString:@"-[pausePhysicsCB]"];
    
    
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(pausePhysicsCB);
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




-(VSC::OB::Scene::Setting) settingForCheckBox:(id)checkBox
{
    /*
    if (checkBox == self.pausePhysicsCheckBox) return VSC::OB::Scene::SettingPaused;
    else if (checkBox == self.drawWireFrameCheckBox) return VSC::OB::Scene::SettingDrawWireFrame;
    else if (checkBox == self.drawAABBCheckBox) return VSC::OB::Scene::SettingDrawAABB;
    else if (checkBox == self.drawContactPointsCheckBox) return VSC::OB::Scene::SettingDrawContactPoints;
    else if (checkBox == self.textCheckBox) return VSC::OB::Scene::SettingDrawText;
    else if (checkBox == self.featureTextCheckBox) return VSC::OB::Scene::SettingDrawFeaturesText;
    else if (checkBox == self.enableBulletLCPICheckBox) return VSC::OB::Scene::SettingBulletLCPIEnabled;
    else if (checkBox == self.enableCCDCheckBox) return VSC::OB::Scene::SettingCCDEnabled;
    else if (checkBox == self.enableSatComparisonCheckBox) return VSC::OB::Scene::SettingSatComparisonEnabled;
    else if (checkBox == self.profileTimingsCheckBox) return VSC::OB::Scene::SettingProfileTimings;
     */
    
    return VSC::OB::Scene::SettingNone;
}

-(NSButton*) checkBoxForSetting:(VSC::OB::Scene::Setting)setting
{
    /*
    switch (setting)
    {
        case VSC::OB::Scene::SettingPaused:
            return self.pausePhysicsCheckBox;
            
        case VSC::OB::Scene::SettingDrawWireFrame:
            return self.drawWireFrameCheckBox;
            
        case VSC::OB::Scene::SettingDrawAABB:
            return self.drawAABBCheckBox;
            
        case VSC::OB::Scene::SettingDrawContactPoints:
            return self.drawContactPointsCheckBox;
            
        case VSC::OB::Scene::SettingDrawText:
            return self.textCheckBox;
            
        case VSC::OB::Scene::SettingDrawFeaturesText:
            return self.featureTextCheckBox;
            
        case VSC::OB::Scene::SettingBulletLCPIEnabled:
            return self.enableBulletLCPICheckBox;
            
        case VSC::OB::Scene::SettingCCDEnabled:
            return self.enableCCDCheckBox;
            
        case VSC::OB::Scene::SettingSatComparisonEnabled:
            return self.enableSatComparisonCheckBox;
            
        case VSC::OB::Scene::SettingProfileTimings:
            return self.profileTimingsCheckBox;
            
        default:
            break;
    }
     */
    
    return nil;
}

-(NSInteger) stateForSceneSetting:(VSC::OB::Scene::Setting)setting
{
    BOOST_ASSERT(self.sceneController);
    
    if (self.sceneController)
    {
        VSC::OB::Scene::SPtr scene = self.sceneController.scene.lock();
        
        BOOST_ASSERT(scene);
        if (scene)
        {
            bool value = scene->getSetting(setting);
            return value ? NSOnState : NSOffState;
        }
    }
    
    return NSOffState;
}

-(void) reloadInterface
{
    /*
    for (NSButton* checkBox in self.allCheckBoxes)
    {
        VSC::OB::Scene::Setting setting = [self settingForCheckBox:checkBox];
        if (setting != VSC::OB::Scene::SettingNone) [checkBox setState:[self stateForSceneSetting:setting]];
        if (checkBox == self.pausePhysicsCheckBox && [self stateForSceneSetting:setting] == NSOnState) NSLog(@"wooaaa");
    }
     */
}

-(void) reloadShootSpeed
{
    /*
    BOOST_ASSERT(self.sceneController);
    if (self.sceneController)
    {
        VSC::Float speed = self.sceneController.shootSpeed;
        [self.shootSpeedSlider setDoubleValue:speed];
        
    }
     */
}

-(void) reloadSetting:(VSC::OB::Scene::Setting)setting
{
    /*
    NSButton* checkBox = (NSButton*)[self checkBoxForSetting:setting];
    
    if (checkBox == self.pausePhysicsCheckBox && [self stateForSceneSetting:setting] == NSOnState)
        NSLog(@"wooaaa");
    
    if (checkBox)
    {
        [checkBox setState:[self stateForSceneSetting:setting]];
    }
     */
}

@end
