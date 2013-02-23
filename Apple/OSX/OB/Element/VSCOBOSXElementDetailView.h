
#import <Cocoa/Cocoa.h>

@class VSCOSXKeyedCheckBoxView;
@class VSCOSXKeyed3FieldView;
@class VSCOSXKeyed4FieldView;

@protocol VSCOBOSXElementController;

@interface VSCOBOSXElementDetailView : NSView <NSTextFieldDelegate>

@property (assign) IBOutlet id<VSCOBOSXElementController> elementController;

@property (weak) IBOutlet NSTextField* nameTextField;
@property (weak) IBOutlet NSTextField* idTextField;

/*
 *  Position/Rotation
 */

@property (nonatomic, strong) VSCOSXKeyed3FieldView* positionFieldView;
@property (nonatomic, strong) VSCOSXKeyed3FieldView* velocityFieldView;
@property (nonatomic, strong) VSCOSXKeyed4FieldView* rotationFieldView;
@property (nonatomic, strong) VSCOSXKeyedCheckBoxView* immobilizedCheckBoxView;

-(void) setupIfNecessary;

-(IBAction) immobilized:(id)sender;

-(void) reloadWholeInterface;
-(void) reloadPositionInterface;
-(void) reloadPositionInterface:(BOOL)critical;

-(void) printUIDescription;

@end
