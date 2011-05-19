//
//  VSCKeyboardSetup.cpp
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 5/16/11.
//

#include "VSCControlSetup.h"
#include "VSCKeyboard.h"

#include <stream>

#define KEYBOARD_ITERATOR   std::map<VSCKeyboardCombination*,VSCKeyboardAction>::iterator
#define MOUSE_ITERATOR      std::map<VSCMouseCombination*,VSCMouseAction>::iterator

VSCControlSetup::~VSCControlSetup() {
    
    for (KEYBOARD_ITERATOR it = m_keyboard_combination_map.begin(); it != m_keyboard_combination_map.end(); it++) {
        delete (it->first);
    }
    
    for (MOUSE_ITERATOR it = m_mouse_combination_map.begin(); it != m_mouse_combination_map.end(); it++) {
        delete (it->first);
    }
    
}

void VSCControlSetup::setToDefault() {
    
    m_interactionStyle = VSCInteractionStyleFPS;
    
    VSCKeyboardCombination* keyComb = NULL;
    
    keyComb = new VSCKeyboardCombination('a', 0);
    
}

void VSCControlSetup::setKeyboardCombinationAction(VSCKeyboardCombination* combination, VSCKeyboardAction action) {
    // remove last key combination
    removeKeyboardCombination(combination);
    // set new key combination
    std::pair<VSCKeyboardCombination*,VSCKeyboardAction> p(combination,action);
    m_keyboard_combination_map.insert(p);
}

void VSCControlSetup::removeKeyboardCombination(VSCKeyboardCombination* combination) {
    KEYBOARD_ITERATOR it;
    for (it = m_keyboard_combination_map.begin(); it != m_keyboard_combination_map.end(); it++) {
        if (*(it->first) == *combination) 
            break;
    }
    if (it != m_keyboard_combination_map.end()) {
        delete (it->first);
        m_keyboard_combination_map.erase(it);
    }
}

VSCKeyboardAction VSCControlSetup::getActionForKeyboardCombination(VSCKeyboardCombination* combination) {
    // we don't want to compare the pointers but the content of the VSCKeyboardCombination class
    for (KEYBOARD_ITERATOR it = m_keyboard_combination_map.begin(); it != m_keyboard_combination_map.end(); it++) {
        if (*(it->first) == *combination) 
            return it->second;
    }
    return VSCKeyboardActionNone;
}


void VSCControlSetup::setMouseCombinationAction(VSCMouseCombination* combination, VSCMouseAction action) {
    // remove last key combination
    removeMouseCombination(combination);
    // set new key combination
    std::pair<VSCMouseCombination*,VSCMouseAction> p(combination,action);
    m_mouse_combination_map.insert(p);
}

void VSCControlSetup::removeMouseCombination(VSCMouseCombination* combination) {
    MOUSE_ITERATOR it;
    for (it = m_mouse_combination_map.begin(); it != m_mouse_combination_map.end(); it++) {
        if (*(it->first) == *combination) 
            break;
    }
    if (it != m_mouse_combination_map.end()) {
        delete (it->first);
        m_mouse_combination_map.erase(it);
    }
}

VSCMouseAction VSCControlSetup::getActionForMouseCombination(VSCMouseCombination* combination) {
    // we don't want to compare the pointers but the content of the VSCKeyboardCombination class
    for (MOUSE_ITERATOR it = m_mouse_combination_map.begin(); it != m_mouse_combination_map.end(); it++) {
        if (*(it->first) == *combination) 
            return it->second;
    }
    return VSCMouseActionDefault;
}

#pragma mark --- Operator <<

ostream& operator<<(ostream& output, const VSCKeyboardCombination& c) {
    output << "VSCKeyboardCombination ('" <<  c.unicode_char << "', " << c.modifier_flags <<")";
    return output;  
}

