//
//  VSCOSXOBSceneElementDetailView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementDetailView.h"
#import "VSCOSXOBSceneElementController.h"

#import "NSString+VSCAdditions.h"

#include "VSCOBScene.h"
#include "OgreBulletDynamicsRigidBody.h"
#include <Ogre/Ogre.h>
#include <boost/assert.hpp>

@interface VSCOSXOBSceneElementDetailView ()

@property (nonatomic, assign) NSTimeInterval minimumInterfaceUpdateInterval;
@property (nonatomic, strong) NSDate* lastUpdateDate;

-(void) commonInit;

-(NSArray*) allNumberTextFields;

@end

@implementation VSCOSXOBSceneElementDetailView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        // Initialization code here.
        [self commonInit];
    }
    return self;
}

-(id) initWithCoder:(NSCoder *)aDecoder
{
    self = [super initWithCoder:aDecoder];
    if (self) {
        // Initialization code here.
        [self commonInit];
    }
    return self;
}

-(void) commonInit
{
    self.translatesAutoresizingMaskIntoConstraints = NO;
    self.minimumInterfaceUpdateInterval = 0.02;
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
}

-(void) awakeFromNib
{
    NSArray* numberTextFields = [self allNumberTextFields];
    
    for (NSTextField* textField in numberTextFields)
    {
        BOOST_ASSERT([textField isKindOfClass:[NSTextField class]]);
        if ([textField isKindOfClass:[NSTextField class]])
        {
            NSNumberFormatter* formatter = textField.formatter;
            BOOST_ASSERT(formatter);
            BOOST_ASSERT([formatter isKindOfClass:[NSNumberFormatter class]]);
            if ([formatter isKindOfClass:[NSNumberFormatter class]])
            {
                formatter.maximumFractionDigits = 2;
                formatter.thousandSeparator = @"";
            }
        }
    }
}

-(NSArray*) allNumberTextFields
{
    return @[
    self.xPosTextField,
    self.yPosTextField,
    self.zPosTextField,
    self.xVelTextField,
    self.yVelTextField,
    self.zVelTextField,
    self.wRotTextField,
    self.xRotTextField,
    self.yRotTextField,
    self.zRotTextField
    ];
}

#pragma mark - UI Helpers

-(void) reloadWholeInterface
{
    [self reloadNameInterface];
    [self reloadImmobilizedInterface];
    [self reloadPositionInterface:YES];
}

-(void) reloadNameInterface
{
    BOOST_ASSERT(self.elementController);
    
    if (self.elementController)
    {
        VSC::OB::Scene::Element::SPtr element = self.elementController.element.lock();
        
        if (element)
        {
            NSString *nameString = [NSString stringWithStdString:element->getName()];
            NSString *idString = [NSString stringWithFormat:@"id: %d", element->getIdentifier()];
            
            [self.nameTextField setStringValue:nameString];
            [self.idTextField setStringValue:idString];
            
            return;
        }
    }
    
    [self.nameTextField setStringValue:@"No Element"];
    [self.idTextField setStringValue:@"id: None"];
}

-(void) reloadImmobilizedInterface
{
    BOOST_ASSERT(self.elementController);
    
    if (self.elementController)
    {
        VSC::OB::Scene::Element::SPtr element = self.elementController.element.lock();
        
        if (element)
        {
            bool immobilized = element->isImmobilized();
            if (immobilized)
            {
                self.immobilizedCheckBox.state = NSOnState;
            }
            else
            {
                self.immobilizedCheckBox.state = NSOffState;
            }
        }
    }
}

-(void) reloadPositionInterface
{
    [self reloadPositionInterface:YES];
}

-(void) reloadPositionInterface:(BOOL)critical
{
    if (!critical &&
        self.lastUpdateDate &&
        [[NSDate date] timeIntervalSinceDate:self.lastUpdateDate] < self.minimumInterfaceUpdateInterval)
    {
        return;
    }
    
    BOOST_ASSERT(self.elementController);
    
    if (self.elementController)
    {
        VSC::OB::Scene::Element::SPtr element = self.elementController.element.lock();
        
        if (element)
        {
            OgreBulletDynamics::RigidBody* rigidBody = element->getRigidBody();
            BOOST_ASSERT(rigidBody);
            if (rigidBody)
            {
                const Ogre::Vector3& linearVelocity = rigidBody->getLinearVelocity();
                
                [self.xVelTextField setDoubleValue:linearVelocity.x];
                [self.yVelTextField setDoubleValue:linearVelocity.y];
                [self.zVelTextField setDoubleValue:linearVelocity.z];
                
                Ogre::Node* node = rigidBody->getSceneNode();
                //BOOST_ASSERT(node);
                if (node)
                {
                    const Ogre::Vector3& position = node->getPosition();
                    
                    [self.xPosTextField setDoubleValue:position.x];
                    [self.yPosTextField setDoubleValue:position.y];
                    [self.zPosTextField setDoubleValue:position.z];
                    
                    const Ogre::Quaternion& rotation = node->getOrientation();
                    
                    [self.xRotTextField setDoubleValue:rotation.x];
                    [self.yRotTextField setDoubleValue:rotation.y];
                    [self.zRotTextField setDoubleValue:rotation.z];
                    [self.wRotTextField setDoubleValue:rotation.w];
                    
                }
                
                else
                {
                    [self.xPosTextField setStringValue:@"N/A"];
                    [self.yPosTextField setStringValue:@"N/A"];
                    [self.zPosTextField setStringValue:@"N/A"];
                    
                    [self.xRotTextField setStringValue:@"N/A"];
                    [self.yRotTextField setStringValue:@"N/A"];
                    [self.zRotTextField setStringValue:@"N/A"];
                    [self.wRotTextField setStringValue:@"N/A"];
                }
            }
            
        }
    }
    
    self.lastUpdateDate = [NSDate date];
}

#pragma mark - UI Callbacks


#pragma mark - NSTextFieldDelegate

- (BOOL)control:(NSControl *)control textShouldEndEditing:(NSText *)fieldEditor
{
    return YES;
}

- (BOOL)control:(NSControl *)control didFailToFormatString:(NSString *)string errorDescription:(NSString *)error
{
    return NO;
}


@end
