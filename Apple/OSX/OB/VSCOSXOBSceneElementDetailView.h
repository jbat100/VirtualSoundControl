
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

@property (nonatomic, strong) VSCOSXKeyed3FieldView* positionFieldView;
@property (nonatomic, strong) VSCOSXKeyed3FieldView* velocityFieldView;
@property (nonatomic, strong) VSCOSXKeyed4FieldView* rotationFieldView;
@property (nonatomic, strong) VSCOSXKeyedCheckBoxView* immobilizedCheckBoxView;

-(IBAction) immobilized:(id)sender;

-(void) reloadWholeInterface;
-(void) reloadPositionInterface;
-(void) reloadPositionInterface:(BOOL)critical;
-(void) reloadImmobilizedInterface;

@end
