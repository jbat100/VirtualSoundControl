//
//  VSCOSXTableSectionView.m
//  VSCApplication
//
//  Created by Jonathan Thorpe on 2/10/13.
//  Copyright (c) 2013 JBAT. All rights reserved.
//

#import "VSCOSXTableSectionView.h"

@implementation VSCOSXTableSectionView

- (id)initWithFrame:(NSRect)frame
{
    self = [super initWithFrame:frame];
    if (self)
    {
        
    }
    
    return self;
}

-(void) awakeFromNib
{
    assert(self.nameTextField);
    self.nameTextField.translatesAutoresizingMaskIntoConstraints = NO;
}

- (void)drawRect:(NSRect)dirtyRect
{
    // Drawing code here.
    CGContextRef myContext = (CGContextRef)[[NSGraphicsContext currentContext] graphicsPort];
    CGContextSetRGBFillColor (myContext, 0.2, 0.2, 0.2, 1.0);
    CGContextFillRect(myContext, self.bounds);
}

+(CGFloat) defaultHeight
{
    return 30.0;
}

@end
