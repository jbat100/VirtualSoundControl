//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementCell.h"

#import "NSString+VSCAdditions.h"

@interface VSCOSXOBSceneElementCell ()

@property (nonatomic, strong) IBOutlet NSTextField* nameTextField;
@property (nonatomic, strong) IBOutlet NSButton* inspectorButton;

-(IBAction) showElementInspector:(id)sender;

@end

@implementation VSCOSXOBSceneElementCell

+(CGFloat) defaultViewHeight
{
    return 60.0;
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
