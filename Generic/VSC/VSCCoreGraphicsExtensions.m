//
//  VSCCoreGraphicsExtensions.m
//  
//
//  Created by Jonathan Thorpe on 10/20/11.
//  Copyright (c) 2011 JBAT. All rights reserved.
//

#import "VSCCoreGraphicsExtensions.h"


/// Align rectangles
//
//  Args:
//    alignee - rect to be aligned
//    aligner - rect to be aligned to
//    alignment - alignment to be applied to alignee based on aligner

CGRect GTMCGAlignRectangles(CGRect alignee, CGRect aligner, GTMRectAlignment alignment) {
    switch (alignment) {
        case GTMRectAlignTop:
            alignee.origin.x = aligner.origin.x + (CGRectGetWidth(aligner) * .5f - CGRectGetWidth(alignee) * .5f);
            alignee.origin.y = aligner.origin.y + CGRectGetHeight(aligner) - CGRectGetHeight(alignee);
            break;
            
        case GTMRectAlignTopLeft:
            alignee.origin.x = aligner.origin.x;
            alignee.origin.y = aligner.origin.y + CGRectGetHeight(aligner) - CGRectGetHeight(alignee);
            break;
            
        case GTMRectAlignTopRight:
            alignee.origin.x = aligner.origin.x + CGRectGetWidth(aligner) - CGRectGetWidth(alignee);
            alignee.origin.y = aligner.origin.y + CGRectGetHeight(aligner) - CGRectGetHeight(alignee);
            break;
            
        case GTMRectAlignLeft:
            alignee.origin.x = aligner.origin.x;
            alignee.origin.y = aligner.origin.y + (CGRectGetHeight(aligner) * .5f - CGRectGetHeight(alignee) * .5f);
            break;
            
        case GTMRectAlignBottomLeft:
            alignee.origin.x = aligner.origin.x;
            alignee.origin.y = aligner.origin.y;
            break;
            
        case GTMRectAlignBottom:
            alignee.origin.x = aligner.origin.x + (CGRectGetWidth(aligner) * .5f - CGRectGetWidth(alignee) * .5f);
            alignee.origin.y = aligner.origin.y;
            break;
            
        case GTMRectAlignBottomRight:
            alignee.origin.x = aligner.origin.x + CGRectGetWidth(aligner) - CGRectGetWidth(alignee);
            alignee.origin.y = aligner.origin.y;
            break;
            
        case GTMRectAlignRight:
            alignee.origin.x = aligner.origin.x + CGRectGetWidth(aligner) - CGRectGetWidth(alignee);
            alignee.origin.y = aligner.origin.y + (CGRectGetHeight(aligner) * .5f - CGRectGetHeight(alignee) * .5f);
            break;
            
        default:
        case GTMRectAlignCenter:
            alignee.origin.x = aligner.origin.x + (CGRectGetWidth(aligner) * .5f - CGRectGetWidth(alignee) * .5f);
            alignee.origin.y = aligner.origin.y + (CGRectGetHeight(aligner) * .5f - CGRectGetHeight(alignee) * .5f);
            break;
    }
    return alignee;
}

CGRect GTMCGScaleRectangleToSize(CGRect scalee, CGSize size, GTMScaling scaling) {
    switch (scaling) {
            
        case GTMScaleToFillProportionally:
        case GTMScaleProportionally: {
            CGFloat height = CGRectGetHeight(scalee);
            CGFloat width = CGRectGetWidth(scalee);
            if (isnormal(height) && isnormal(width) && 
                (height > size.height || width > size.width)) {
                CGFloat horiz = size.width / width;
                CGFloat vert = size.height / height;
                BOOL expand = (scaling == GTMScaleToFillProportionally);
                // We use the smaller scale unless expand is true. In that case, larger.
                CGFloat newScale = ((horiz < vert) ^ expand) ? horiz : vert;
                scalee = GTMCGRectScale(scalee, newScale, newScale);
            }
            break;
        }
            
        case GTMScaleToFit:
            scalee.size = size;
            break;
            
        case GTMScaleNone:
        default:
            // Do nothing
            break;
    }
    return scalee;
}