

#import "VSCOSXKeyed1FieldView.h"

@implementation VSCOSXKeyed1FieldView

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
    assert(self.valueTextField);
    
    self.labelTextField.translatesAutoresizingMaskIntoConstraints = NO;
    self.valueTextField.translatesAutoresizingMaskIntoConstraints = NO;
}

-(NSArray*) allValueTextFields
{
    return @[self.valueTextField];
}

@end
