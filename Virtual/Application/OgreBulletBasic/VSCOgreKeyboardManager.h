
#ifndef _VSC_OGRE_KEYBOARD_MANAGER_H_
#define _VSC_OGRE_KEYBOARD_MANAGER_H_

#include "VSCUserInput.h"
#include "VSCKeyBindings.h"
#include "VSCOgreKeyboardAction.h"

#include <boost/shared_ptr.hpp>

class VSCOgreKeyboardManager;

typedef boost::shared_ptr<VSCOgreKeyboardManager> VSCOgreKeyboardManagerPtr;

class VSCOgreKeyboardManager
{
    
public:
    
    VSCOgreKeyboardManager();
    ~VSCOgreKeyboardManager();
    
    VSCOgreKeyboardAction::Key  getActionKeyForCombination(const VSCKeyboard::Combination& comb) const;
    VSCKeyboard::Combination    getCombinationForActionKey(const VSCOgreKeyboardAction::Key& key)  const;
    
    void setBinding(const VSCOgreKeyboardAction::Key& key, const VSCKeyboard::Combination& comb);
    
    void loadDefaultBindings(void);
    
private:
    
    VSCKeyBindings mKeyBindings;
    
};

#endif//_VSC_OGRE_KEYBOARD_MANAGER_H_

