//
//  VSCIMOSXMappingEditViewController.m
//  OgreBulletCocoaTestApplications
//
//  Created by Jonathan Thorpe on 10/31/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#import "VSCIMOSXMappingEditViewController.h"
#import "VSCOSXInterfaceFactory.h"
#import "VSCOSXKeyed1FieldView.h"

#include "VSCIMMapping.h"
#include <boost/assert.hpp>

using namespace VSC;
using namespace VSC::IM;

@interface VSCIMOSXMappingEditViewController ()

@property (nonatomic, strong) NSArray* setupConstraints;

-(void) reloadInterface;
-(void) resetView;
-(void) setupNumericTextFieldNumberFormatter:(NSTextField*)textField;

// binding targets

@property (nonatomic, assign) Float offset;
@property (nonatomic, assign) Float scaleFactor;

@end

@implementation VSCIMOSXMappingEditViewController

- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil
{
    self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil];
    if (self)
    {
        // Initialization code here.
    }
    
    return self;
}


#pragma mark - Custom Setters

-(void) setOffset:(Float)offset
{
    _offset = offset;
    
    Mapping::SPtr mapping = _mapping.lock();
    BOOST_ASSERT(mapping);
    if (mapping)
    {
        mapping->setOffset(offset);
    }
}

-(void) setScaleFactor:(Float)scaleFactor
{
    _scaleFactor = scaleFactor;
    
    Mapping::SPtr mapping = _mapping.lock();
    BOOST_ASSERT(mapping);
    if (mapping)
    {
        mapping->setScaleFactor(scaleFactor);
    }
}

-(void) setMapping:(Mapping::WPtr)mapping
{
    if (mapping.lock() != _mapping.lock())
    {
        _mapping = mapping;
        [self reloadInterface];
    }
}

#pragma mark - UI Helpers

-(void) resetView
{
    NSArray* subviews = [self.view subviews];
    [subviews makeObjectsPerformSelector:@selector(removeFromSuperview)];
    if (self.setupConstraints)
    {
        [self.view removeConstraints:self.setupConstraints];
    }
}

-(void) reloadInterface
{
    [self resetView];
    
    self.setupConstraints = nil;
    
    Mapping::SPtr mapping = _mapping.lock();
    BOOST_ASSERT(mapping);
    if (mapping)
    {
        self.scaleFactor = mapping->getScaleFactor();
        self.offset = mapping->getOffset();
        
        VSCOSXInterfaceFactory* factory = [VSCOSXInterfaceFactory defaultFactory];
        
        NSMutableArray* viewsForHorizontalConstraints = [NSMutableArray array];
        NSMutableString* verticalVisualFormat = [NSMutableString stringWithString:@"V:|"];
        
        id owner = nil;
        NSView* editView = [self view];
        
        self.offsetKeyedFieldView = [factory newKeyed1FieldViewWithOwner:owner];
        [self.offsetKeyedFieldView.labelTextField setTextColor:[NSColor whiteColor]];
        [self setupNumericTextFieldNumberFormatter:self.offsetKeyedFieldView.valueTextField];
        [editView addSubview:self.offsetKeyedFieldView];
        
        [self.offsetKeyedFieldView.valueTextField bind:@"value" toObject:self withKeyPath:@"offset" options:nil];
        [self.offsetKeyedFieldView.valueTextField setEditable:YES];
        [self.offsetKeyedFieldView.valueTextField setAcceptsTouchEvents:YES];
        [self.offsetKeyedFieldView.valueTextField setEnabled:YES];
        [self.offsetKeyedFieldView.valueTextField setDelegate:self];
        
        [self.offsetKeyedFieldView.labelTextField setStringValue:@"Offset"];
        NSView* offsetView = self.offsetKeyedFieldView;
        [viewsForHorizontalConstraints addObject:offsetView];
        [verticalVisualFormat appendString:@"-[offsetView]"];
        
        self.scaleFactorKeyedFieldView = [factory newKeyed1FieldViewWithOwner:owner];
        [self.scaleFactorKeyedFieldView.labelTextField setTextColor:[NSColor whiteColor]];
        [self setupNumericTextFieldNumberFormatter:self.scaleFactorKeyedFieldView.valueTextField];
        [editView addSubview:self.scaleFactorKeyedFieldView];
        [self.scaleFactorKeyedFieldView.valueTextField bind:@"value" toObject:self withKeyPath:@"scaleFactor" options:nil];
        [self.scaleFactorKeyedFieldView.valueTextField setEditable:YES];
        [self.scaleFactorKeyedFieldView.labelTextField setStringValue:@"Scale Factor"];
        NSView* scaleFactorView = self.scaleFactorKeyedFieldView;
        [viewsForHorizontalConstraints addObject:scaleFactorView];
        [verticalVisualFormat appendString:@"-[scaleFactorView]"];
        
        [verticalVisualFormat appendString:@"-|"];
        NSDictionary *viewsDictionary = NSDictionaryOfVariableBindings(offsetView, scaleFactorView);
        
        NSLog(@"VerticalVisualFormat: %@", verticalVisualFormat);
        
        NSMutableArray* newSetupConstraints = [NSMutableArray array];
        
        NSArray* vConstraints = [NSLayoutConstraint constraintsWithVisualFormat:verticalVisualFormat
                                                                        options:0
                                                                        metrics:nil
                                                                          views:viewsDictionary];
        [editView addConstraints:vConstraints];
        [newSetupConstraints addObjectsFromArray:vConstraints];
        
        for (NSView* viewForConstraint in viewsForHorizontalConstraints)
        {
            NSDictionary *localViewsDictionary = NSDictionaryOfVariableBindings(viewForConstraint);
            NSArray* hConstraints = [NSLayoutConstraint constraintsWithVisualFormat:@"H:|-35-[viewForConstraint]-|"
                                                                            options:0
                                                                            metrics:nil
                                                                              views:localViewsDictionary];
            [editView addConstraints:hConstraints];
            [newSetupConstraints addObjectsFromArray:hConstraints];
        }
        
        self.setupConstraints = [NSArray arrayWithArray:newSetupConstraints];
    }
}

-(void) setupNumericTextFieldNumberFormatter:(NSTextField*)textField
{
    BOOST_ASSERT([textField isKindOfClass:[NSTextField class]]);
    if ([textField isKindOfClass:[NSTextField class]])
    {
        // don't change the font here, disallows editing for some very strange reason 
        //textField.font = [NSFont fontWithName:@"System" size:11];
        
        NSNumberFormatter* formatter = textField.formatter;
        BOOST_ASSERT(formatter);
        BOOST_ASSERT([formatter isKindOfClass:[NSNumberFormatter class]]);
        if ([formatter isKindOfClass:[NSNumberFormatter class]])
        {
            formatter.minimumFractionDigits = 2;
            formatter.maximumFractionDigits = 2;
            formatter.thousandSeparator = @"";
        }
    }
}

#pragma mark - NSTextFieldDelegate

-(BOOL)control:(NSControl *)control textShouldBeginEditing:(NSText *)fieldEditor
{
    NSLog(@"%@ textShouldBeginEditing %@", control, fieldEditor);
    return YES;
}

- (BOOL)control:(NSControl *)control textShouldEndEditing:(NSText *)fieldEditor
{
    NSLog(@"%@ textShouldEndEditing %@", control, fieldEditor);
    return YES;
}

@end
