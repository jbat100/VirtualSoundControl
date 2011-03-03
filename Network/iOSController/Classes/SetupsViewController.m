//
//  SetupsViewController.m
//  iOSController
//
//  Created by Jonathan Thorpe on 28/02/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import "SetupsViewController.h"


@implementation SetupsViewController

@synthesize setupTableViewCell, setupsTableView, setups;

// The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
/*
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self) {
        // Custom initialization.
    }
    return self;
}
*/

/*
// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
}
*/

/*
// Override to allow orientations other than the default portrait orientation.
- (BOOL)shouldAutorotateToInterfaceOrientation:(UIInterfaceOrientation)interfaceOrientation {
    // Return YES for supported orientations.
    return (interfaceOrientation == UIInterfaceOrientationPortrait);
}
*/

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];
    
    // Release any cached data, images, etc. that aren't in use.
}

- (void)viewDidUnload {
    [super viewDidUnload];
    // Release any retained subviews of the main view.
    // e.g. self.myOutlet = nil;
}


- (void)dealloc {
    [super dealloc];
}

#pragma mark -
#pragma mark UITableView Delegate/Datasource

-(UITableViewCell *) tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
	
	static NSString* identifier = @"SetupTableViewCellIdentifier";
	
    SetupTableViewCell* cell = (SetupTableViewCell*)[tableView dequeueReusableCellWithIdentifier:identifier];
    if (cell == nil) {
        [[NSBundle mainBundle] loadNibNamed:@"SetupTableViewCell" owner:self options:nil];
        cell = setupTableViewCell;
        self.setupTableViewCell = nil;
    }
	
	if ([indexPath section] == 0 && [indexPath row] < [setupManager.presetSetups count]) 
		cell.setup = [setupManager.presetSetups objectAtIndex:[indexPath row]];
	else if ([indexPath section] == 1 && [indexPath row] < [setupManager.userSetups count]) 
		cell.setup = [setupManager.userSetups objectAtIndex:[indexPath row]];
	else 
		cell.setup = nil;
	
	[cell update];
		
    return cell;
	
}

-(void) tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
	
}

-(NSString *) tableView:(UITableView *)tableView titleForHeaderInSection:(NSInteger)section {
	if (section == 0)
		return @"Presets";
	else if (section == 1)
		return @"User Setups";
	return @"";
}

-(NSInteger) numberOfSectionsInTableView:(UITableView *)tableView {
	return 2;
}

-(NSInteger) tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
	if (section == 0)
		return [setupManager.presetSetups count];
	else if (section == 1)
		return [setupManager.userSetups count];
	return 0;
}

-(CGFloat) tableView:(UITableView *)tableView heightForRowAtIndexPath:(NSIndexPath *)indexPath {
	return 70.0;
}


@end
