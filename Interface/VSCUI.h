
#ifndef _VSC_USER_INPUT_H_
#define _VSC_USER_INPUT_H_

#include <boost/bimap.hpp>
#include <string>
#include <set>
#include "OIS.h"

typedef std::set<OIS::KeyCode>          VSCKeyCodeSet;
typedef std::set<OIS::MouseButtonID>    VSCMouseButtonSet;

class VSCKeyboard 
{
    
public:
    
    struct Combination {
        OIS::Keyboard::Modifier modifier;
        OIS::KeyCode code;
        bool operator!=(const Combination& p) const;
        bool operator==(const Combination& p) const;
        bool operator<(const Combination& p) const;
        Combination(OIS::KeyCode c, OIS::Keyboard::Modifier m = (OIS::Keyboard::Modifier)0);
    };
    
    static std::string stringForKeyCode(const OIS::KeyCode& code);
    static std::string stringForModifier(const OIS::Keyboard::Modifier& modifier);
    static std::string stringForCombination(const Combination& comb);

    static const Combination  NullCombination;
    
};


#endif//_VSC_USER_INPUT_H_

