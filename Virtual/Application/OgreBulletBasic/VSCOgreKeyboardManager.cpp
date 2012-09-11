
#include "VSCOgreKeyboardManager.h"
#include "VSCUserInput.h"
#include "VSCKeyBindings.h"
#include "VSCOgreKeyboardAction.h"

#include <boost/assert.hpp>

VSCOgreKeyboardManager::VSCOgreKeyboardManager()
{
    this->loadDefaultBindings();
}

VSCOgreKeyboardManager::~VSCOgreKeyboardManager()
{
    
}

VSCOgreKeyboardAction::Key VSCOgreKeyboardManager::actionKeyForCombination(const VSCKeyboard::Combination& comb) const 
{
    VSCKeyboard::Action action = mKeyBindings.getActionForCombination(comb);
    if (action == VSCKeyboard::NullAction) return VSCOgreKeyboardAction::None;
    BOOST_ASSERT_MSG(action.domain == VSCKeyboard::ActionDomainOgre, "Expected action.domain to be VSCKeyboard::ActionDomainOgre");
    return (VSCOgreKeyboardAction::Key)action.key;
}

VSCKeyboard::Combination VSCOgreKeyboardManager::combinationForActionKey(const VSCOgreKeyboardAction::Key& key) const 
{
    VSCKeyboard::Action action(key, VSCKeyboard::ActionDomainOgre);
    VSCKeyboard::Combination comb = mKeyBindings.getCombinationForAction(action);
    return comb;
}

void VSCOgreKeyboardManager::setBinding(const VSCOgreKeyboardAction::Key& key, const VSCKeyboard::Combination& comb)
{
    VSCKeyboard::Action action(key, VSCKeyboard::ActionDomainOgre);
    mKeyBindings.setBinding(action, comb);
}

void VSCOgreKeyboardManager::loadDefaultBindings(void)
{
    this
}


