//
//  VSCOgreView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 6/22/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "OgreOSXCocoaView.h"

class VSCOgreBulletCocoaInputAdapter;

@interface VSCOgreView : OgreView {
    
    
}

@property (nonatomic, assign) VSCOgreBulletCocoaInputAdapter* inputAdapter;

@end
