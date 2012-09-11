
#include "VSCUserInput.h"

const VSCKeyboard::Action       VSCKeyboard::NullAction      = {VSCKeyboard::ActionDomainNone, 0};
const VSCKeyboard::Combination  VSCKeyboard::NullCombination = {(OIS::Keyboard::Modifier)0, OIS::KC_UNASSIGNED};


bool VSCKeyboard::Combination::operator!=(const Combination& p) const
{
    return !(*this == p);
}

bool VSCKeyboard::Combination::operator==(const Combination& p) const
{
    return modifier == p.modifier && code == p.code;
}

bool VSCKeyboard::Combination::operator<(const Combination& p) const
{
    if (modifier < p.modifier) return true;
    if (modifier > p.modifier) return false;
    if (code < p.code) return true;
    return false;
}

VSCKeyboard::Combination(OIS::KeyCode c, OIS::Keyboard::Modifier m) : code(c), modifier(m)
{
    
}

bool VSCKeyboard::Action::operator!=(const Action& a) const
{
    return !(*this == a);
}

bool VSCKeyboard::Action::operator==(const Action& a) const
{
    return domain == a.domain && key == a.key;
}

bool VSCKeyboard::Action::operator<(const Action& a) const
{
    if (domain < a.domain) return true;
    if (domain > a.domain) return false;
    if (key < a.key) return true;
    return false;
}

VSCKeyboard::Action(int k, ActionDomain d) : key(k), domain(d)
{
    
}