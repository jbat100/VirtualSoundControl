//
//  VSCGUI.h
//  EnveloppeEditor
//
//  Created by Jonathan Thorpe on 4/4/12.
//  Copyright (c) 2012 JBAT. All rights reserved.
//

#ifndef _VSC_GUI_H_
#define _VSC_GUI_H_

struct {
    float x;
    float y;
} VSCGUIPoint;

struct {
    float width;
    float height;
} VSCGUISize;

struct {
    VSCGUIPoint origin;
    VSCGUISize  size;
} VSCGUIRect;


#endif
