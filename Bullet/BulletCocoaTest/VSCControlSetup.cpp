//
//  VSCKeyboardSetup.cpp
//  BulletCocoaTest
//
//  Created by Jonathan Thorpe on 5/16/11.
//

#include "VSCControlSetup.h"

//#ifdef __APPLE__
//#include "NSEvent.h"
//#endif

void VSCControlSetup::setToDefault() {
    
    m_interactionStyle = VSCInteractionStyleFPS;
    
}

void VSCControlSetup::setKeyboardCombinationAction(VSCKeyboardCombination combination, VSCKeyboardAction action) {
    // remove last key combination (multimap)
    removeKeyCombination(combination);
    // set new key combination
    //m_key_combination_map.insert(std::pair<VSCKeyboardCombination, VSCKeyboardAction>(combination, action));
    test_map.insert(std::pair<int, int>(3,3)); 
}

void VSCControlSetup::removeKeyCombination(VSCKeyboardCombination combination) {
    //m_key_combination_map.erase(m_key_combination_map.find(combination));
}

VSCKeyboardAction VSCControlSetup::getActionForKeyCombination(VSCKeyboardCombination combination) {
    
    /*
    std::map<VSCKeyboardCombination,VSCKeyboardAction>::iterator it = m_key_combination_map.find(combination);
    if (it != m_key_combination_map.end()) 
        return it->second;
    */
    return VSCKeyboardActionNone;
}