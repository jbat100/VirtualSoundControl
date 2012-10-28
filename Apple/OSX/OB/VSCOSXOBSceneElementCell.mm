//
//  Created by Jonathan Thorpe on 10/28/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCOSXOBSceneElementCell.h"

#import "NSString+VSCAdditions.h"

@implementation VSCOSXOBSceneElementCell

-(void) showDetail:(BOOL)detailed
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
    
    std::string name = elem->getName();
    if (!name.empty()) [self.nameTextField setStringValue:[NSString stringWithStdString:name]];
    else [self.nameTextField setStringValue:@"Un-named"];
    
    
}

-(IBAction) showElementInspector
{
    [self.environmentController showElementInspectorForElement:self.element.lock()];
}

@end
