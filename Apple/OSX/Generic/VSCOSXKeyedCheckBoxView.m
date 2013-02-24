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
    //CGContextRef myContext = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    //CGContextSetGrayFillColor (myContext, 0.4, 1.0);
    //CGContextFillRect(myContext, NSRectToCGRect(self.bounds));
    
}

- (void)awakeFromNib
{
    assert(self.checkBoxButton);
    //assert(self.labelTextField);
    
    self.checkBoxButton.translatesAutoresizingMaskIntoConstraints = NO;
    //self.labelTextField.translatesAutoresizingMaskIntoConstraints = NO;
}

@end
