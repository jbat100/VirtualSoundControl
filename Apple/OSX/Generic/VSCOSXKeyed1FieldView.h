

#import <Cocoa/Cocoa.h>

@interface VSCOSXKeyed1FieldView : NSView

@property (nonatomic, strong) IBOutlet NSTextField* labelTextField;
@property (nonatomic, strong) IBOutlet NSTextField* valueTextField;

-(NSArray*) allValueTextFields;

@end
