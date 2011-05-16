//
//  VSCKeyboardSetup.h
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 5/16/11.
//


#ifndef VSC_CONTROL_SETUP_H
#define VSC_CONTROL_SETUP_H

#include <map>

enum VSCMouseButton {
	VSCMouseButtonNone = 0,
	VSCMouseButtonRight,
	VSCMouseButtonCenter,
	VSCMouseButtonLeft
};

// bitmask to allow concurrent button presses where needed, never know...
enum VSCMouseAction {
	VSCMouseActionNone = 0,
	VSCMouseActionDefault = 1<<0, // camera except if clicked on world object, then grab
	VSCMouseActionCamera = 1<<1,
	VSCMouseActionGrab = 1<<2,
	VSCMouseActionShootBox = 1<<3,
	VSCMouseActionZoom = 1<<4
};

enum VSCInteractionStyle {
    VSCInteractionStyleNone = 0,
	VSCInteractionStyleDefault,
	VSCInteractionStyleFPS,
	VSCInteractionStyleCentered
};

enum VSCKeyboardAction {
    VSCKeyboardActionNone = 0,
    VSCKeyboardActionMoveRight = 1001,
    VSCKeyboardActionMoveLeft = 1002,
    VSCKeyboardActionMoveForward = 1003,
    VSCKeyboardActionMoveBackward = 1004,
    VSCKeyboardActionMoveUp = 1005,
    VSCKeyboardActionMoveDown = 1006
};

struct VSCKeyboardCombination {
    VSCKeyboardCombination() : unicode_char (NULL), modifier_flags (0) {}
    wchar_t unicode_char;
    unsigned int modifier_flags;
    bool operator==(VSCKeyboardCombination &other) { return (unicode_char == other.unicode_char && modifier_flags == other.modifier_flags); }
    bool operator!=(VSCKeyboardCombination &other) { return !(*this == other); } 
    bool operator<(VSCKeyboardCombination &other) { return unicode_char < other.unicode_char; } 
};

struct VSCMouseCombination {
    VSCMouseCombination() : button (VSCMouseButtonNone), modifier_flags (0) {}
    VSCMouseButton button;
    unsigned int modifier_flags;
    bool operator==(VSCMouseCombination &other) { return (button == other.button && modifier_flags == other.modifier_flags); }
    bool operator!=(VSCMouseCombination &other) { return !(*this == other); } 
    bool operator<(VSCMouseCombination &other) { return button < other.button; }
};

class VSCControlSetup {
	
protected:
    
    VSCInteractionStyle m_interactionStyle;
    
    std::map<int,int> test_map;
    
    std::map<VSCMouseCombination, VSCMouseAction> m_mouse_combination_map;
    std::map<VSCKeyboardCombination, VSCKeyboardAction> m_key_combination_map;
    
public:
    
    void setToDefault();
    
    void setKeyboardCombinationAction(VSCKeyboardCombination combination, VSCKeyboardAction action);
    void removeKeyCombination(VSCKeyboardCombination combination);
    VSCKeyboardAction getActionForKeyCombination(VSCKeyboardCombination combination);
    
};


#endif