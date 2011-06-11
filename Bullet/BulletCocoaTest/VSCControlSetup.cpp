//
//  VSCKeyboardSetup.cpp
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 5/16/11.
//

#include "VSCControlSetup.h"
#include "VSCKeyboard.h"

#include <iostream>
#include <sstream>
#include <string>


#define KEYBOARD_ITERATOR   std::map<VSCKeyboardCombination*,VSCKeyboardAction>::iterator
#define KEYBOARD_PAIR		std::pair<VSCKeyboardCombination*,VSCKeyboardAction>

#define MOUSE_ITERATOR      std::map<VSCMouseCombination*,VSCMouseAction>::iterator
#define MOUSE_PAIR			std::pair<VSCMouseCombination*,VSCMouseAction>

std::string wideCharString(wchar_t wc) {
	
	std::stringstream wcss;
	for (int i = 0; i < sizeof(wc); i++) {
	}
	
	int wci = (int)wc;
	
	wcss << std::hex << wci;
	
	return wcss.str();
	
}


bool VSCKeyboardCombination::operator==(VSCKeyboardCombination &other)
{ 
	//std::wcout << "Evaluating == between " << *this << " and " << other;
	
	unsigned int vsc_modifier_flags_mask = VSCModifierMask;
	unsigned int other_vsc_modifier_flags = other.modifier_flags & vsc_modifier_flags_mask;
	unsigned int this_vsc_modifier_flags = (*this).modifier_flags & vsc_modifier_flags_mask;
	if (!other_vsc_modifier_flags == this_vsc_modifier_flags) 
		return false;
	
	return (unicode_char == other.unicode_char); 
}

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
    
    /*
     * KEYS
     */
    
    keyComb = new VSCKeyboardCombination(L'a', 0);
	setKeyboardCombinationAction(keyComb, VSCKeyboardActionMoveLeft);
	
	keyComb = new VSCKeyboardCombination(L'w', 0);
	setKeyboardCombinationAction(keyComb, VSCKeyboardActionMoveForward);
	
	keyComb = new VSCKeyboardCombination(L's', 0);
	setKeyboardCombinationAction(keyComb, VSCKeyboardActionMoveBackward);
	
	keyComb = new VSCKeyboardCombination(L'd', 0);
	setKeyboardCombinationAction(keyComb, VSCKeyboardActionMoveRight);
	
	keyComb = new VSCKeyboardCombination((wchar_t)VSCUpArrowFunctionKey, 0);
	setKeyboardCombinationAction(keyComb, VSCKeyboardActionMoveForward);
	
	keyComb = new VSCKeyboardCombination((wchar_t)VSCDownArrowFunctionKey, 0);
	setKeyboardCombinationAction(keyComb, VSCKeyboardActionMoveBackward);
	
	keyComb = new VSCKeyboardCombination((wchar_t)VSCLeftArrowFunctionKey, 0);
	setKeyboardCombinationAction(keyComb, VSCKeyboardActionMoveLeft);
	
	keyComb = new VSCKeyboardCombination((wchar_t)VSCRightArrowFunctionKey, 0);
	setKeyboardCombinationAction(keyComb, VSCKeyboardActionMoveRight);
    
    
    /*
     * MOUSE
     */
	
	VSCMouseCombination* mouseComb = NULL;
	
	mouseComb = new VSCMouseCombination(VSCMouseButtonLeft, 0);
	setMouseCombinationAction(mouseComb, VSCMouseActionDefault);
	
	/*
	std::cout << "\nModifiers: ";
	std::cout << "\nVSCAlphaShiftKeyMask: " << std::hex << VSCAlphaShiftKeyMask;
	std::cout << "\nVSCShiftKeyMask: " << std::hex << VSCShiftKeyMask;
	std::cout << "\nVSCControlKeyMask: " << std::hex << VSCControlKeyMask;
	std::cout << "\nVSCAlternateKeyMask: " << std::hex << VSCAlternateKeyMask;
	std::cout << "\nVSCCommandKeyMask: " << std::hex << VSCCommandKeyMask << std::endl;
	 */
    
}

void VSCControlSetup::setKeyboardCombinationAction(VSCKeyboardCombination* combination, VSCKeyboardAction action) {
    // remove last key combination
    removeKeyboardCombination(combination);
    // set new key combination
    KEYBOARD_PAIR p(combination,action);
    m_keyboard_combination_map.insert(p);
}

void VSCControlSetup::removeKeyboardCombination(VSCKeyboardCombination* combination) {
    KEYBOARD_ITERATOR it;
    for (it = m_keyboard_combination_map.begin(); it != m_keyboard_combination_map.end(); it++) {
        if (*(it->first) == *combination) {
            break;
		}
    }
    if (it != m_keyboard_combination_map.end()) {
        delete (it->first);
        m_keyboard_combination_map.erase(it);
    }
}

VSCKeyboardAction VSCControlSetup::getActionForKeyboardCombination(VSCKeyboardCombination* combination) {
    // we don't want to compare the pointers but the content of the VSCKeyboardCombination class
	//std::wcout << "\nGetting action for " << *combination;
    for (KEYBOARD_ITERATOR it = m_keyboard_combination_map.begin(); it != m_keyboard_combination_map.end(); it++) {
		//std::wcout << "\nComparing with " << *(it->first);
        if (*(it->first) == *combination) {
			//std::cout << ": IS EQUAL! ";
            return it->second;
		}
		else {
			//std::cout << ": not equal :( ... ";
		}

    }
    return VSCKeyboardActionNone;
}


void VSCControlSetup::setMouseCombinationAction(VSCMouseCombination* combination, VSCMouseAction action) {
    // remove last key combination
    removeMouseCombination(combination);
    // set new key combination
    MOUSE_PAIR p(combination,action);
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
	
std::wstring modifierString(unsigned int modifier_flags) {
	
	unsigned int vsc_modifier_flags_mask = VSCModifierMask;
	unsigned int vsc_modifier_flags = modifier_flags & vsc_modifier_flags_mask;
	
	std::wstring mod_string;
	
	if (vsc_modifier_flags & (unsigned int)VSCAlphaShiftKeyMask)
		mod_string.append(L"|VSCAlphaShiftKeyMask");
	if (vsc_modifier_flags & (unsigned int)VSCShiftKeyMask)
		mod_string.append(L"|VSCShiftKeyMask");
	if (vsc_modifier_flags & (unsigned int)VSCControlKeyMask)
		mod_string.append(L"|VSCControlKeyMask");
	if (vsc_modifier_flags & (unsigned int)VSCAlternateKeyMask)
		mod_string.append(L"|VSCAlternateKeyMask");
	if (vsc_modifier_flags & (unsigned int)VSCCommandKeyMask)
		mod_string.append(L"|VSCCommandKeyMask");
	if (mod_string.size() == 0)
		mod_string.append(L"NONE");
		
	return mod_string;  
}

std::wostream& operator<<(std::wostream& output, const VSCKeyboardCombination& c) {
    output << "VSCKeyboardCombination ('" <<  c.unicode_char << "', modifiers: " << c.modifier_flags <<":";
	output << modifierString(c.modifier_flags) << ")";
    return output;  
}

std::wostream& operator<<(std::wostream& output, const VSCMouseCombination& c) {
    output << "VSCMouseCombination ('" <<  c.button << "', modifiers: " << c.modifier_flags <<")";
	output << modifierString(c.modifier_flags) << ")";
    return output;  
}

