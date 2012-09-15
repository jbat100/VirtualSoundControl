
#ifndef _VSC_KEY_BINDINGS_H_
#define _VSC_KEY_BINDINGS_H_

#include "VSCUI.h"
#include <boost/bimap.hpp>
#include <string>
#include <set>
#include "OIS.h"

/*
 *  VSCKeyBindings stores and manipulates a group of bindings between VSCKeyboard::Action and 
 *  VSCKeyboard::Combination
 */

class VSCKeyBindings 
{
    
public:
    
    /*
     *  Get the Action/Combination associated with a Combination/Action. Returns NullAction/NullCombination
     *  if none was found.
     */
    
    const VSCKeyboard::Action getActionForCombination(const VSCKeyboard::Combination& comb) const;
    const VSCKeyboard::Combination getCombinationForAction(const VSCKeyboard::Action& action) const;
    
    /*
     *  Erase the binding for a given Action or Combination. 
     */
    
    void eraseCombination(const VSCKeyboard::Combination& comb);
    void eraseAction(const VSCKeyboard::Action& action);
    
    /*
     *  Set a binding, throws an exception if the action and/or the combination is already bound.
     */
    
    void setBinding(const VSCKeyboard::Action& action, const VSCKeyboard::Combination& comb);
    
private:
    
    typedef boost::bimap<VSCKeyboard::Combination, VSCKeyboard::Action>     CombinationActionMap;
    typedef CombinationActionMap::value_type                                CombinationActionMapEntry;
    typedef CombinationActionMap::left_map::iterator                        CombinationIterator;
    typedef CombinationActionMap::right_map::iterator                       ActionIterator;
    typedef CombinationActionMap::left_map::const_iterator                  CombinationConstIterator;
    typedef CombinationActionMap::right_map::const_iterator                 ActionConstIterator;
    
    CombinationActionMap    mCombinationActionMap;
    
};


#endif//_VSC_KEY_BINDINGS_H_

