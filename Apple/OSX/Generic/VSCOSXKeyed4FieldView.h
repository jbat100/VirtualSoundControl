//
//  VSCOSXKeyed4FieldView.h
//  VSCApplication
//
//  Created by Jonathan Thorpe on 1/20/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import <Cocoa/Cocoa.h>

@interface VSCOSXKeyed4FieldView : NSView

@property (nonatomic, strong) IBOutlet NSTextField* labelTextField;

@property (nonatomic, strong) IBOutlet NSTextField* value1TextField;
@property (nonatomic, strong) IBOutlet NSTextField* value2TextField;
@property (nonatomic, strong) IBOutlet NSTextField* value3TextField;
@property (nonatomic, strong) IBOutlet NSTextField* value4TextField;

-(NSArray*) allValueTextFields;

@end
