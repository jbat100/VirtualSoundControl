
#import <Cocoa/Cocoa.h>

@class VSCOSXKeyedCheckBoxView;
@class VSCOSXKeyed3FieldView;
@class VSCOSXKeyed4FieldView;

@protocol VSCOBOSXElementController;

@interface VSCOBOSXElementDetailView : NSView <NSTextFieldDelegate>

@property (assign) IBOutlet id<VSCOBOSXElementController> elementController;

@property (nonatomic, strong) IBOutlet NSTextField* nameTextField;
@property (nonatomic, strong) IBOutlet NSTextField* idTextField;

//@property (nonatomic, strong) IBOutlet NSButton* printElementDescriptionButton;
@property (nonatomic, strong) IBOutlet NSButton* printVertexDeclarationsButton;
@property (nonatomic, strong) IBOutlet NSButton* printVerticesButton;
@property (nonatomic, strong) IBOutlet NSButton* printIndecesButton;
@property (nonatomic, strong) IBOutlet NSButton* printNormalsButton;
@property (nonatomic, strong) IBOutlet NSButton* printDiffuseColorsButton;
@property (nonatomic, strong) IBOutlet NSButton* printSpecularColorsButton;
@property (nonatomic, strong) IBOutlet NSButton* printTextureCoordinatesButton;

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
