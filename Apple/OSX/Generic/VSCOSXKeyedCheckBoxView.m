//
//  VSCKeyedCheckBoxView.m
//  VSCApplication
//
//  Created by Jonathan Thorpe on 1/18/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import "VSCOSXKeyedCheckBoxView.h"

@implementation VSCOSXKeyedCheckBoxView

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

- (void)awakeFromNib
{
    assert(self.checkBoxButton);
    assert(self.labelTextField);
    
    self.checkBoxButton.translatesAutoresizingMaskIntoConstraints = NO;
    self.labelTextField.translatesAutoresizingMaskIntoConstraints = NO;
}

@end
