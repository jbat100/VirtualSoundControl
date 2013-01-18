//
//  VSCOSXKeyedSliderView.m
//  VSCApplication
//
//  Created by Jonathan Thorpe on 1/18/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import "VSCOSXKeyedSliderView.h"

@implementation VSCOSXKeyedSliderView

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
    assert(self.slider);
    assert(self.labelTextField);
    assert(self.valueTextField);
    
    self.slider.translatesAutoresizingMaskIntoConstraints = NO;
    self.labelTextField.translatesAutoresizingMaskIntoConstraints = NO;
    self.valueTextField.translatesAutoresizingMaskIntoConstraints = NO;
}

@end
