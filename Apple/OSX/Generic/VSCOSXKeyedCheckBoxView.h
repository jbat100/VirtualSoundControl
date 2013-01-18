//
//  VSCKeyedCheckBoxView.h
//  VSCApplication
//
//  Created by Jonathan Thorpe on 1/18/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface VSCOSXKeyedCheckBoxView : NSView

@property (nonatomic, weak) NSButton* checkBoxButton;
@property (nonatomic, weak) NSTextField* labelTextField;

@end
