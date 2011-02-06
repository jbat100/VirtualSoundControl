//
//  UITouch+JSONPackaging.h
//  iOSController
//
//  Created by Jonathan Thorpe on 27/01/2011.
//  Copyright 2011 JBAT. All rights reserved.
//

#import <UIKit/UIKit.h>


@interface UITouch (JSONPackaging)

-(NSDictionary*) jsonPackageDictionary;

-(NSDictionary*) jsonPackageDictionaryCentered:(BOOL)centered normalised:(BOOL)normalised inverted:(BOOL)inverted;

@end
