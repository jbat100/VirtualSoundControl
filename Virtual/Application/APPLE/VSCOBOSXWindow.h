//
//  VSCOgreBulletWindow.h
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 7/12/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@class VSCOgreView;

class VSC::OB::Application;

@interface VSCOgreBulletWindow : NSWindow

@property (nonatomic, strong) IBOutlet VSCOgreView* ogreView;
@property (nonatomic, strong) IBOutlet NSPopUpButton* scenePopUpButton;

@property (nonatomic, assign) VSC::OB::Application* ogreBulletApplication;

- (IBAction)test1:(id)sender;
- (IBAction)test2:(id)sender;
- (IBAction)test3:(id)sender;

- (IBAction)scenePopUpButtonChanged:(id)sender;

@end
