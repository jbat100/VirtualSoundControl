//
//  VSCOSXOBSceneElementView.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 12/3/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementView.h"

#import "NSString+VSCAdditions.h"

@interface VSCOSXOBSceneElementView ()

@property (nonatomic, strong) IBOutlet NSTextField* nameTextField;
@property (nonatomic, strong) IBOutlet NSButton* inspectorButton;

-(IBAction) showElementInspector:(id)sender;

@end


@implementation VSCOSXOBSceneElementView

+(CGFloat) defaultViewHeight
{
    return 60.0;
}

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


-(void) awakeFromNib
{
    
}

-(void) setElement:(VSC::OB::Scene::Element::WPtr)elem
{
    _element = elem;
    [self reload];
}

-(void) reload
{
    VSC::OB::Scene::Element::SPtr elem = self.element.lock();
    BOOST_ASSERT(elem);
    if (elem)
    {
        std::stringstream displayName;
        displayName << elem->getName() << "(id: " << elem->getIdentifier() << ")";
        [self.nameTextField setStringValue:[NSString stringWithStdString:displayName.str()]];
    }
    else
    {
        [self.nameTextField setStringValue:@"EMPTY"];
    }
    
    
}

-(IBAction) showElementInspector:(id)sender
{
    [self.environmentController showElementInspectorForElement:self.element.lock()];
}

@end
