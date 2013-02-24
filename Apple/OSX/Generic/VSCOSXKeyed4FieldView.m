//
//  VSCOSXKeyed4FieldView.m
//  VSCApplication
//
//  Created by Jonathan Thorpe on 1/20/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import "VSCOSXKeyed4FieldView.h"

@implementation VSCOSXKeyed4FieldView

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

- (void) awakeFromNib
{
    assert(self.labelTextField);
    assert(self.value1TextField);
    assert(self.value2TextField);
    assert(self.value3TextField);
    assert(self.value4TextField);
    
    self.labelTextField.translatesAutoresizingMaskIntoConstraints = NO;
    self.value1TextField.translatesAutoresizingMaskIntoConstraints = NO;
    self.value2TextField.translatesAutoresizingMaskIntoConstraints = NO;
    self.value3TextField.translatesAutoresizingMaskIntoConstraints = NO;
    self.value4TextField.translatesAutoresizingMaskIntoConstraints = NO;
}

-(NSArray*) allValueTextFields
{
    return @[self.value1TextField, self.value2TextField, self.value3TextField, self.value4TextField];
}

@end
