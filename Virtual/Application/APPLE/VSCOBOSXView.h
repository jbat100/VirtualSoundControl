//
//  VSCOBOSXView.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 6/22/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "OgreOSXCocoaView.h"

namespace VSC { namespace OB {
    class OSXInputAdapter;
}}

@interface VSCOBOSXView : OgreView {
    
    
}

@property (nonatomic, assign) VSC::OB::OSXInputAdapter* inputAdapter;

@end
