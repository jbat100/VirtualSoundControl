
#include "VSCUI.h"
#include "OIS.h"

#include <string>
#include <set>

const VSC::Keyboard::Combination  VSC::Keyboard::NullCombination = VSC::Keyboard::Combination(OIS::KC_UNASSIGNED, (OIS::Keyboard::Modifier)0);


bool VSC::Keyboard::Combination::operator!=(const Combination& p) const
{
    return !(*this == p);
}

bool VSC::Keyboard::Combination::operator==(const Combination& p) const
{
    return modifier == p.modifier && code == p.code;
}

bool VSC::Keyboard::Combination::operator<(const Combination& p) const
{
    if (modifier < p.modifier) return true;
    if (modifier > p.modifier) return false;
    if (code < p.code) return true;
    return false;
}

VSC::Keyboard::Combination::Combination(OIS::KeyCode c, OIS::Keyboard::Modifier m) : code(c), modifier(m)
{
    
}
