//
//  VSCOBSceneElementListController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneController.h"
#import "VSCOBOSXSceneDisplayView.h"
#import "VSCOSXOBSceneDetailView.h"
#import "VSCOSXOBSceneElementListView.h"
#import "VSCOSXOBSceneElementView.h"

#include "VSCOBSceneController.h"
#include "VSCOBScene.h"
#include "VSCOBElement.h"
#include "VSCEnvironment.h"
#include "VSCEnvironmentTest.h"

static const Ogre::Vector3 CameraStartPos = Ogre::Vector3(30.0, 10.0, 0.0);
static const Ogre::Vector3 CameraStartLookAt = Ogre::Vector3(0.0, 0.0, 0.0);

@interface VSCOSXOBSceneController ()

@end

@implementation VSCOSXOBSceneController

static const BOOL traceInterface = YES;

@synthesize scene = _scene;
@synthesize environmentController = _environmentController;

@synthesize elementListView = _elementListView;
@synthesize sceneView = _sceneView;
@synthesize sceneDetailView = _sceneDetailView;

@synthesize shootSpeed = _shootSpeed;
@synthesize cameraSpeed = _cameraSpeed;
@synthesize cameraSensitivity = _cameraSensitivity;

@synthesize pausePhysics = _pausePhysics;
@synthesize drawWireFrame = _drawWireFrame;
@synthesize drawAABB = _drawAABB;
@synthesize drawContactPoints = _drawContactPoints;
@synthesize text = _text;
@synthesize featureText = _featureText;
@synthesize enableBulletLCPI = _enableBulletLCPI;
@synthesize enableCCD = _enableCCD;
@synthesize enableSatComparison = _enableSatComparison;
@synthesize profileTimings = _profileTimings;

-(id) init {
    
    self = [super init];
    if (self)
    {
        self.sceneListener = VSC::OB::OSXSceneListener::SPtr(new VSC::OB::OSXSceneListener);
        self.sceneListener->setTarget(self);
    }
    
    return self;
    
}

-(void) dealoc
{
    NSLog(@"%@ DEALLOC", self);
}

#pragma mark - Scene Helpers

-(void) resetSceneDisplayViews
{
    BOOST_ASSERT(self.sceneView);
    
    if (self.sceneView)
    {
        VSC::OB::Display::SPtr display = self.sceneView.display.lock();
    }
    
}

#pragma mark - Custom Setters

-(void) setScene:(VSC::OB::Scene::WPtr)s
{
    BOOST_ASSERT(self.sceneListener);
    BOOST_ASSERT(self.sceneListener->getTarget() == self);
    
    VSC::OB::Scene::SPtr oldScene = _scene.lock();
    if (oldScene && self.sceneListener)
    {
        oldScene->removeListener(self.sceneListener);
    }
    
    _scene = s;
    
    [self.sceneView setupControllerChain];
    
    VSC::OB::Scene::SPtr newScene = _scene.lock();
    if (newScene && self.sceneListener)
    {
        newScene->addListener(self.sceneListener);
    }
    
    [self.elementListView.elementTableView reloadData];
}

-(void) setShootSpeed:(VSC::Float)shootSpeed
{
    _shootSpeed = shootSpeed;
    BOOST_ASSERT(self.sceneView.internalSceneController);
    if (self.sceneView.internalSceneController)
    {
        self.sceneView.internalSceneController->setShootSpeed(shootSpeed);
    }
}

-(void) setCameraSpeed:(VSC::Float)cameraSpeed
{
    _cameraSpeed = cameraSpeed;
    BOOST_ASSERT(self.sceneView.displayController);
    if (self.sceneView.displayController)
    {
        self.sceneView.displayController->setCameraSpeed(cameraSpeed);
    }
}

-(void) setCameraSensitivity:(VSC::Float)cameraSensitivity
{
    _cameraSensitivity = cameraSensitivity;
    BOOST_ASSERT(self.sceneView.displayController);
    if (self.sceneView.displayController)
    {
        self.sceneView.displayController->setMouseSensitivity(cameraSensitivity);
    }
}

-(void) setPausePhysics:(BOOL)pausePhysics
{
    _pausePhysics = pausePhysics;
    VSC::OB::Scene::SPtr s = self.scene.lock();
    BOOST_ASSERT(s);
    if (s)
    {
        s->setSetting(VSC::OB::Scene::SettingPaused, pausePhysics);
    }
}

#pragma mark - UI Callbacks


-(IBAction)checkBoxAction:(id)sender
{
    /*
     
    VSC::OB::Scene::SPtr s = self.scene.lock();
    BOOST_ASSERT(s); if (!s) return;
    
    BOOST_ASSERT(self.sceneDetailView);
    
    VSC::OB::Scene::Setting setting = [self.sceneDetailView settingForCheckBox:sender];
    BOOST_ASSERT(setting != VSC::OB::Scene::SettingNone); if (setting == VSC::OB::Scene::SettingNone) return;
    
    bool value = ([(NSButton*)sender state] == NSOnState) ? true : false;
    s->setSetting(setting, value);
     
     */
}

-(IBAction)shootSpeedSliderAction:(id)sender
{
    /*
    
    BOOST_ASSERT(sender == self.sceneDetailView.shootSpeedSlider);
    if (sender == self.sceneDetailView.shootSpeedSlider)
    {

    }
     
     */
}

-(IBAction)cameraSpeedSliderAction:(id)sender
{
    
}

-(IBAction)cameraSensitivitySliderAction:(id)sender
{
    
}

-(IBAction)resetAction:(id)sender
{
    //[self setupTest];
}

#pragma mark - VSCOBOSXSceneListenerTarget Methods

-(void) scene:(VSC::OB::Scene::SPtr)scene registeredElement:(VSC::OB::Element::SPtr)element
{
    [self.elementListView.elementTableView reloadData];
}

-(void) scene:(VSC::OB::Scene::SPtr)scene changedSetting:(VSC::OB::Scene::Setting)setting toValue:(BOOL)value
{
    [self.sceneDetailView reloadSetting:setting];
}

#pragma mark - PXListViewDelegate Methods

- (NSInteger)numberOfRowsInTableView:(NSTableView *)aTableView
{
    VSC::OB::Scene::SPtr s = self.scene.lock();
    
    if (s)
    {
        VSC::OB::Elements& elems = s->getElements();
        NSLog(@"%@ numberOfRowsInListView %ld", self, (NSUInteger)(elems.size()));
        return (NSUInteger)(elems.size());
    }
    
    NSLog(@"%@ numberOfRowsInListView NO SCENE", self);
    
	return 0;
}

- (NSView *)tableView:(NSTableView *)tableView viewForTableColumn:(NSTableColumn*)tableColumn row:(NSInteger)row
{
    BOOST_ASSERT(tableView == self.elementListView.elementTableView);
    
    if (tableView == self.elementListView.elementTableView)
    {
        if (traceInterface) NSLog(@"%@ listView:cellForRow: %ld", self, row);
        
        VSC::OB::Scene::SPtr s = self.scene.lock();
        VSC::OB::Elements& elems = s->getElements();
        BOOST_ASSERT((NSInteger)elems.size() > row);
        if ((NSInteger)elems.size() > row)
        {
            VSC::OB::Element::SPtr elem = elems.at(row);
            VSC::OB::Element::WPtr weakElem = VSC::OB::Element::WPtr(elem);
            VSCOSXOBSceneElementView* elementView = [tableView makeViewWithIdentifier:[[VSCOSXOBSceneElementView class] description] owner:self];
            BOOST_ASSERT(elementView);
            BOOST_ASSERT([elementView isKindOfClass:[VSCOSXOBSceneElementView class]]);
            elementView.environmentController = self.environmentController;
            elementView.element = weakElem;
            
            return elementView;
        }
    }
    
	return nil;
}

- (CGFloat)tableView:(NSTableView *)tableView heightOfRow:(NSInteger)row
{
    BOOST_ASSERT(tableView == self.elementListView.elementTableView);
    
    if (tableView == self.elementListView.elementTableView)
    {
        return [VSCOSXOBSceneElementView defaultViewHeight];
    }
    
    return 0;
}


@end
