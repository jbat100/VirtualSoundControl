//
//  VSCOSXOBSceneDetailView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 11/17/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneDetailView.h"
#import "VSCOSXOBSceneController.h"

#include "VSCOBScene.h"

@interface VSCOSXOBSceneDetailView ()

-(NSInteger) stateForSceneSetting:(VSC::OB::Scene::Setting)setting;

@property (nonatomic, strong) NSArray* allCheckBoxes;

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
    NSMutableArray* checkBoxes = [NSMutableArray array];
    
    [checkBoxes addObject:self.pausePhysicsCheckBox];
    [checkBoxes addObject:self.drawWireFrameCheckBox];
    [checkBoxes addObject:self.drawAABBCheckBox];
    [checkBoxes addObject:self.drawContactPointsCheckBox];
    [checkBoxes addObject:self.textCheckBox];
    [checkBoxes addObject:self.featureTextCheckBox];
    [checkBoxes addObject:self.enableBulletLCPICheckBox];
    [checkBoxes addObject:self.enableCCDCheckBox];
    [checkBoxes addObject:self.enableSatComparisonCheckBox];
    [checkBoxes addObject:self.profileTimingsCheckBox];
    
    self.allCheckBoxes = [NSArray arrayWithArray:checkBoxes];
    
}


-(VSC::OB::Scene::Setting) settingForCheckBox:(id)checkBox
{
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
    
    return VSC::OB::Scene::SettingNone;
}

-(NSButton*) checkBoxForSetting:(VSC::OB::Scene::Setting)setting
{
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
    for (NSButton* checkBox in self.allCheckBoxes)
    {
        VSC::OB::Scene::Setting setting = [self settingForCheckBox:checkBox];
        if (setting != VSC::OB::Scene::SettingNone) [checkBox setState:[self stateForSceneSetting:setting]];
        if (checkBox == self.pausePhysicsCheckBox && [self stateForSceneSetting:setting] == NSOnState)
            NSLog(@"wooaaa");
    }
}

-(void) reloadSetting:(VSC::OB::Scene::Setting)setting
{
    NSButton* checkBox = (NSButton*)[self checkBoxForSetting:setting];
    
    if (checkBox == self.pausePhysicsCheckBox && [self stateForSceneSetting:setting] == NSOnState)
        NSLog(@"wooaaa");
    
    if (checkBox)
    {
        [checkBox setState:[self stateForSceneSetting:setting]];
    }
}

@end
