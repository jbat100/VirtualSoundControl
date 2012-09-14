
#ifndef _VSC_OGRE_KEY_BINDINGS_H_
#define _VSC_OGRE_KEY_BINDINGS_H_

#include "VSCUI.h"  
#include "VSCBindings.h"
#include "VSCOgreKeyboardAction.h"
#include <set>

class VSCOgreKeyBindings : private VSCBindings<VSCOgreKeyboardAction::Key, VSCKeyboard::Combination> 
{
    VSCOgreKeyBindings() {}
    virtual ~VSCOgreKeyBindings() {}
    
    std::set<VSCOgreKeyboardAction::Key>    getActionKeysForCombination(void);
    std::set<VSCKeyboard::Combination>      getCombinationForActionKeys(void);
    
};

class VSCOgreKeyBound : private VSCBound<VSCOgreKeyboardAction::Key, VSCKeyboard::Combination> 
{
    VSCOgreKeyBound() {}
    virtual ~VSCOgreKeyBound() {}
}

#endif//_VSC_OGRE_KEY_BINDINGS_H_

