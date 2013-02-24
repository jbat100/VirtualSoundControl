//
//  VSCOBOSXElementDetailView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOBOSXElementDetailView.h"
#import "VSCOBOSXElementController.h"
#import "VSCOSXInterfaceFactory.h"

#import "VSCOSXKeyedCheckBoxView.h"
#import "VSCOSXKeyed3FieldView.h"
#import "VSCOSXKeyed4FieldView.h"

#import "NSString+VSCAdditions.h"

#include "VSCOBScene.h"
#include "VSCOBElement.h"
#include "VSCOBDynamicObject.h"

#include "OgreBulletDynamicsRigidBody.h"

#include <Ogre/Ogre.h>

#include <boost/assert.hpp>

using namespace VSC;
using namespace VSC::IM;
using namespace VSC::OB;

@interface VSCOBOSXElementDetailView ()
{

}

@property (nonatomic, assign) NSTimeInterval minimumInterfaceUpdateInterval;
@property (nonatomic, strong) NSDate* lastUpdateDate;
@property (nonatomic, assign) BOOL isSetup;

-(void) setupNumericTextFieldNumberFormatter:(NSTextField*)textField;

-(void) commonInit;

-(NSArray*) allNumberTextFields;

@end

@implementation VSCOBOSXElementDetailView

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
    self.isSetup = NO;
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
    CGContextRef myContext = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    CGContextSetGrayFillColor (myContext, 0.8, 1.0);
    CGContextFillRect(myContext, NSRectToCGRect(self.bounds));
}

-(void) setupIfNecessary
{

        
    VSCOSXInterfaceFactory* factory = [VSCOSXInterfaceFactory defaultFactory];
    
    NSMutableArray* viewsForHorizontalConstraints = [NSMutableArray array];
    NSMutableString* verticalVisualFormat = [NSMutableString stringWithString:@"V:|"];
    
    id owner = nil;
    
    /*
     *  Title
     */
    
    /*
     *  Check Boxes
     */
    
    //self.immobilizedCheckBoxView = [factory newKeyedCheckBoxViewWithOwner:self.elementController];
    self.immobilizedCheckBoxView = [factory newKeyedCheckBoxViewWithOwner:owner];
    [self addSubview:self.immobilizedCheckBoxView];
    [self.immobilizedCheckBoxView.checkBoxButton bind:@"value" toObject:self.elementController withKeyPath:@"immobilized" options:nil];
    [self.immobilizedCheckBoxView.checkBoxButton setTitle:@"Immobilized"];
    NSView* immobilizedCB = self.immobilizedCheckBoxView;
    [viewsForHorizontalConstraints addObject:immobilizedCB];
    [verticalVisualFormat appendString:@"-[immobilizedCB]"];
    
    /*
     *  Field Views
     */
    
    self.positionFieldView = [factory newKeyed3FieldViewWithOwner:owner];
    [self addSubview:self.positionFieldView];
    [self.positionFieldView.labelTextField setStringValue:@"Position"];
    [self.positionFieldView.value1TextField setDelegate:self];
    
    self.positionFieldView.value1TextField.editable = YES;
    self.positionFieldView.value1TextField.selectable = YES;
    self.positionFieldView.value1TextField.enabled = YES;
    
    //self.positionFieldView.canBecomeKeyView = YES;
    //self.positionFieldView.value1TextField.canBecomeKeyView = YES;
    NSView* positionFV = self.positionFieldView;
    [viewsForHorizontalConstraints addObject:positionFV];
    [verticalVisualFormat appendString:@"-2-[positionFV]"];
    
    self.velocityFieldView = [factory newKeyed3FieldViewWithOwner:owner];
    [self addSubview:self.velocityFieldView];
    [self.velocityFieldView.labelTextField setStringValue:@"Velocity"];
    NSView* velocityFV = self.velocityFieldView;
    [viewsForHorizontalConstraints addObject:velocityFV];
    [verticalVisualFormat appendString:@"-2-[velocityFV]"];
    
    self.rotationFieldView = [factory newKeyed4FieldViewWithOwner:owner];
    [self addSubview:self.rotationFieldView];
    [self.rotationFieldView.labelTextField setStringValue:@"Rotation"];
    NSView* rotationFV = self.rotationFieldView;
    [viewsForHorizontalConstraints addObject:rotationFV];
    [verticalVisualFormat appendString:@"-2-[rotationFV]"];
    
    /*
     *  Debug buttons
     */
    
    self.printElementDescriptionButton = [[NSButton alloc] init];
    self.printElementDescriptionButton.translatesAutoresizingMaskIntoConstraints = NO;
    [self addSubview:self.printElementDescriptionButton];
    NSView* printElementDescriptionButton = self.printElementDescriptionButton;
    [self.printElementDescriptionButton setTitle: @"Print Element Description"];
    [self.printElementDescriptionButton setTarget:self];
    [self.printElementDescriptionButton setAction:@selector(printElementDescription:)];
    [self.printElementDescriptionButton setButtonType:NSMomentaryLightButton];
    [self.printElementDescriptionButton setBezelStyle:NSTexturedSquareBezelStyle];
    [viewsForHorizontalConstraints addObject:printElementDescriptionButton];
    [verticalVisualFormat appendString:@"-20-[printElementDescriptionButton(==20)]"];
    
    /*
     *  End and constraint creations
     */
    
    [verticalVisualFormat appendString:@"-(>=10)-|"];
    NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(immobilizedCB, positionFV,
                                                                   velocityFV, rotationFV, printElementDescriptionButton);
    
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
    
    
    NSArray* numberTextFields = [self allNumberTextFields];
    for (NSTextField* textField in numberTextFields)
    {
        [self setupNumericTextFieldNumberFormatter:textField];
    }

}

-(void) awakeFromNib
{

}

-(void) setupNumericTextFieldNumberFormatter:(NSTextField*)textField
{
    BOOST_ASSERT([textField isKindOfClass:[NSTextField class]]);
    if ([textField isKindOfClass:[NSTextField class]])
    {
        // NSTextField font stops the editing WTF
        //textField.font = [NSFont fontWithName:@"System" size:11];
        
        NSNumberFormatter* formatter = textField.formatter;
        BOOST_ASSERT(formatter);
        BOOST_ASSERT([formatter isKindOfClass:[NSNumberFormatter class]]);
        if ([formatter isKindOfClass:[NSNumberFormatter class]])
        {
            formatter.minimumSignificantDigits = 3;
            formatter.minimumFractionDigits = 2;
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

#pragma mark - UI Debug

-(void) printUIDescription
{
    NSLog(@"self.positionFieldView %@ %@", self.positionFieldView, NSStringFromRect(self.positionFieldView.frame));
    NSLog(@"self.velocityFieldView %@ %@", self.velocityFieldView, NSStringFromRect(self.velocityFieldView.frame));
    NSLog(@"self.rotationFieldView %@ %@", self.rotationFieldView, NSStringFromRect(self.rotationFieldView.frame));
}

#pragma mark - UI Helpers

-(void) reloadWholeInterface
{
    [self reloadNameInterface];
    [self reloadPositionInterface:YES];
}

-(void) reloadNameInterface
{
    BOOST_ASSERT(self.elementController);
    
    if (self.elementController)
    {
        Element::SPtr element = self.elementController.element.lock();
        
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
        Element::SPtr element = self.elementController.element.lock();
        
        if (element)
        {
            OgreBulletDynamics::RigidBody* rigidBody = element->getRigidBody();
            BOOST_ASSERT(rigidBody);
            if (rigidBody)
            {
                const Ogre::Vector3& linearVelocity = rigidBody->getLinearVelocity();
                
                [self.velocityFieldView.value1TextField setDoubleValue:linearVelocity.x];
                [self.velocityFieldView.value2TextField setDoubleValue:linearVelocity.y];
                [self.velocityFieldView.value3TextField setDoubleValue:linearVelocity.z];
                
                Ogre::Node* node = rigidBody->getSceneNode();
                //BOOST_ASSERT(node);
                if (node)
                {
                    const Ogre::Vector3& position = node->getPosition();
                    
                    [self.positionFieldView.value1TextField setDoubleValue:position.x];
                    [self.positionFieldView.value2TextField setDoubleValue:position.y];
                    [self.positionFieldView.value3TextField setDoubleValue:position.z];
                    
                    const Ogre::Quaternion& rotation = node->getOrientation();
                    
                    [self.rotationFieldView.value1TextField setDoubleValue:rotation.x];
                    [self.rotationFieldView.value2TextField setDoubleValue:rotation.y];
                    [self.rotationFieldView.value3TextField setDoubleValue:rotation.z];
                    [self.rotationFieldView.value4TextField setDoubleValue:rotation.w];
                    
                }
                
                else
                {
                    [self.positionFieldView.value1TextField setStringValue:@"N/A"];
                    [self.positionFieldView.value2TextField setStringValue:@"N/A"];
                    [self.positionFieldView.value3TextField setStringValue:@"N/A"];
                    
                    [self.rotationFieldView.value1TextField setStringValue:@"N/A"];
                    [self.rotationFieldView.value2TextField setStringValue:@"N/A"];
                    [self.rotationFieldView.value3TextField setStringValue:@"N/A"];
                    [self.rotationFieldView.value4TextField setStringValue:@"N/A"];
                }
            }
            
        }
    }
    
    self.lastUpdateDate = [NSDate date];
}

#pragma mark - UI Callbacks

-(IBAction) printElementDescription:(id)sender
{
    NSLog(@"Print Element Description");
    
    Element::SPtr element = self.elementController.element.lock();
    
    DynamicObject::SPtr dynamicObject = boost::dynamic_pointer_cast<DynamicObject>(element);
    if (dynamicObject)
    {
        
    }
}


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
