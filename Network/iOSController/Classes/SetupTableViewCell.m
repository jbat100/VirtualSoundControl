//
//  SetupTableViewCell.m
//  iOSController
//
//  Created by Jonathan Thorpe on 28/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "SetupTableViewCell.h"
#import "ControlCenterSetup.h"

@implementation SetupTableViewCell

@synthesize setup, setupSchemaView, descriptorLabel;

- (id)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    
    self = [super initWithStyle:style reuseIdentifier:reuseIdentifier];
    if (self) {
        // Initialization code.
    }
    return self;
}


- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    
    [super setSelected:selected animated:animated];
    
    // Configure the view for the selected state.
}


- (void)dealloc {
    [super dealloc];
}

-(void) update {
	
	descriptorLabel.text = setup.descriptor;
	setupSchemaView.setup = setup;
	[setupSchemaView setNeedsDisplay];
	
}


@end
