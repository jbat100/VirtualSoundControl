
#ifndef _VSC_USER_INPUT_H_
#define _VSC_USER_INPUT_H_

#include <boost/bimap.hpp>
#include <string>
#include <set>
#include <ostream>
#include "OIS.h"

namespace VSC {

    typedef std::set<OIS::KeyCode>          KeyCodeSet;
    typedef std::set<OIS::MouseButtonID>    MouseButtonSet;

    class Keyboard 
    {
        
    public:
        
        struct Combination
        {
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
    
    std::ostream& operator << (std::ostream& stream, const Keyboard::Combination& comb);
    
    class Mouse
    {
      
    public:
        
        struct Combination
        {
            OIS::Keyboard::Modifier modifier;
            OIS::MouseButtonID button;
            bool operator!=(const Combination& p) const;
            bool operator==(const Combination& p) const;
            bool operator<(const Combination& p) const;
            Combination(OIS::MouseButtonID b, OIS::Keyboard::Modifier m = (OIS::Keyboard::Modifier)0);
        };
        
        static const Combination NullCombination;
    };
    
}


#endif//_VSC_USER_INPUT_H_

