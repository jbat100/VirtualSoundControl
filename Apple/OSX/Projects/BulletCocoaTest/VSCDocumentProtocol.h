//
//  VSCDocumentProtocol.h
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 13/06/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

class VSCRootApplication;


@protocol VSCDocumentProtocol

-(VSCRootApplication*)rootApplication;
-(void) drewGL;

@end
