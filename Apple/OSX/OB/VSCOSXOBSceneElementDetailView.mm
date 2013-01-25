//
//  VSCOSXOBSceneElementDetailView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementDetailView.h"
#import "VSCOSXOBSceneElementController.h"
#import "VSCOSXInterfaceFactory.h"

#import "VSCOSXKeyedCheckBoxView.h"
#import "VSCOSXKeyed3FieldView.h"
#import "VSCOSXKeyed4FieldView.h"

#import "NSString+VSCAdditions.h"

#include "VSCOBScene.h"
#include "OgreBulletDynamicsRigidBody.h"
#include <Ogre/Ogre.h>
#include <boost/assert.hpp>

@interface VSCOSXOBSceneElementDetailView ()

@property (nonatomic, assign) NSTimeInterval minimumInterfaceUpdateInterval;
@property (nonatomic, strong) NSDate* lastUpdateDate;

-(void) setupNumericTextFieldNumberFormatter:(NSTextField*)textField;

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
    VSCOSXInterfaceFactory* factory = [VSCOSXInterfaceFactory defaultFactory];
    
    NSMutableArray* viewsForHorizontalConstraints = [NSMutableArray array];
    NSMutableString* verticalVisualFormat = [NSMutableString stringWithString:@"V:|"];
    
    /*
     *  Title
     */
    
    /*
     *  Check Boxes
     */
    
    self.immobilizedCheckBoxView = [factory newKeyedCheckBoxViewWithOwner:self.elementController];
    [self addSubview:self.immobilizedCheckBoxView];
    [self.immobilizedCheckBoxView.checkBoxButton bind:@"value" toObject:self.elementController withKeyPath:@"immobilized" options:nil];
    [self.immobilizedCheckBoxView.checkBoxButton setTitle:@"Immobilized"];
    NSView* immobilizedCB = self.immobilizedCheckBoxView;
    [viewsForHorizontalConstraints addObject:immobilizedCB];
    [verticalVisualFormat appendString:@"-[immobilizedCB]"];
    
    /*
     *  Field Views
     */
    
    self.positionFieldView = [factory newKeyed3FieldViewWithOwner:self.elementController];
    [self addSubview:self.positionFieldView];
    [self.positionFieldView.labelTextField setStringValue:@"Position"];
    NSView* positionFV = self.positionFieldView;
    [viewsForHorizontalConstraints addObject:positionFV];
    [verticalVisualFormat appendString:@"-2-[positionFV]"];
    
    self.velocityFieldView = [factory newKeyed3FieldViewWithOwner:self.elementController];
    [self addSubview:self.velocityFieldView];
    [self.velocityFieldView.labelTextField setStringValue:@"Velocity"];
    NSView* velocityFV = self.velocityFieldView;
    [viewsForHorizontalConstraints addObject:velocityFV];
    [verticalVisualFormat appendString:@"-2-[velocityFV]"];
    
    self.rotationFieldView = [factory newKeyed4FieldViewWithOwner:self.elementController];
    [self addSubview:self.rotationFieldView];
    [self.rotationFieldView.labelTextField setStringValue:@"Rotation"];
    NSView* rotationFV = self.rotationFieldView;
    [viewsForHorizontalConstraints addObject:rotationFV];
    [verticalVisualFormat appendString:@"-2-[rotationFV]"];
    
    [verticalVisualFormat appendString:@"-|"];
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(positionFV);
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
    
    
    NSArray* numberTextFields = [self allNumberTextFields];
    for (NSTextField* textField in numberTextFields)
    {
        [self setupNumericTextFieldNumberFormatter:textField];
    }
}

-(void) setupNumericTextFieldNumberFormatter:(NSTextField*)textField
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

-(NSArray*) allNumberTextFields
{
    NSMutableArray* textFields = [NSMutableArray arrayWithCapacity:10];
    [textFields addObjectsFromArray:[self.positionFieldView allValueTextFields]];
    [textFields addObjectsFromArray:[self.velocityFieldView allValueTextFields]];
    [textFields addObjectsFromArray:[self.rotationFieldView allValueTextFields]];
    return [NSArray arrayWithArray:textFields];
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
        VSC::OB::Element::SPtr element = self.elementController.element.lock();
        
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
        VSC::OB::Element::SPtr element = self.elementController.element.lock();
        
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
        VSC::OB::Element::SPtr element = self.elementController.element.lock();
        
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
