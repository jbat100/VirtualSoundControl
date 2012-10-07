
#include "VSCUI.h"
#include "OIS.h"

#include <string>
#include <set>

std::ostream& VSC::operator << (std::ostream& stream, const VSC::Keyboard::Combination& comb)
{
    stream << "Combination: (";
    
    switch (comb.code)
    {
        case OIS::KC_W:
            stream << "W";
            break;
        default:
            stream << "Unknown code";
            break;
    }
    
    stream << " : ";
    
    if (comb.modifier == 0)
    {
        stream << "No modifier";
    }
    else
    {
        stream << "Unknown modifier";
    }
    
    stream << ")";
    
    return stream;
};

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
