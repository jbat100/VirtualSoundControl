//
//  VSCOSXOBSceneElementDetailView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementDetailView.h"
#import "VSCOSXOBSceneElementController.h"

#include "VSCOBScene.h"
#include "OgreBulletDynamicsRigidBody.h"
#include <Ogre/Ogre.h>
#include <boost/assert.hpp>

@implementation VSCOSXOBSceneElementDetailView

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
    // Drawing code here.
}

-(void) awakeFromNib
{
    
}

#pragma mark - UI Helpers

-(void) reloadInterface
{
    BOOST_ASSERT(self.elementController);
    
    VSC::OB::Scene::Element::SPtr element = self.elementController.element.lock();
    
    if (element)
    {
        OgreBulletDynamics::RigidBody* rigidBody = element->getRigidBody();
        BOOST_ASSERT(rigidBody);
        if (rigidBody)
        {
            const Ogre::Vector3& linearVelocity = rigidBody->getLinearVelocity();
            
            //[self.xVelTextField setStringValue:[@(linearVelocity.x) stringValue]];
            //[self.yVelTextField setStringValue:[@(linearVelocity.y) stringValue]];
            //[self.zVelTextField setStringValue:[@(linearVelocity.z) stringValue]];
            
            [self.xVelTextField setDoubleValue:linearVelocity.x];
            [self.yVelTextField setDoubleValue:linearVelocity.y];
            [self.zVelTextField setDoubleValue:linearVelocity.z];
            
            Ogre::Node* node = rigidBody->getSceneNode();
            BOOST_ASSERT(node);
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
        }
        
    }
    
    /*
     *  Reach here if there is no info to display...
     */
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
