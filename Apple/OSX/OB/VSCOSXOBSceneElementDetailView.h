
#import <Cocoa/Cocoa.h>

@class VSCOSXKeyedCheckBoxView;
@class VSCOSXKeyed3FieldView;
@class VSCOSXKeyed4FieldView;

@protocol VSCOSXOBSceneElementController;

@interface VSCOSXOBSceneElementDetailView : NSView

@property (assign) IBOutlet id<VSCOSXOBSceneElementController> elementController;

@property (weak) IBOutlet NSTextField* nameTextField;
@property (weak) IBOutlet NSTextField* idTextField;

/*
 *  Position/Rotation
 */

@property (weak) VSCOSXKeyed3FieldView* position3FieldView;
@property (weak) VSCOSXKeyed3FieldView* velocity3FieldView;
@property (weak) VSCOSXKeyed4FieldView* rotation4FieldView;
@property (weak) VSCOSXKeyedCheckBoxView* immobilizedCheckBoxView;

-(IBAction) immobilized:(id)sender;

-(void) reloadWholeInterface;
-(void) reloadPositionInterface;
-(void) reloadPositionInterface:(BOOL)critical;
-(void) reloadImmobilizedInterface;

@end
