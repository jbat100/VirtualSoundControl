
#include "VSCKeyBindings.h"

const VSCKeyboard::Action VSCKeyBindings::getActionForCombination(const VSCKeyboard::Combination& comb) const
{
    CombinationConstIterator it = mCombinationActionMap.left.find(comb);
    
    if (it != mCombinationActionMap.left.end())
    {
        return it->second;
    }
        
    return VSCKeyboard::NullAction;
}

const VSCKeyboard::Combination VSCKeyBindings::getCombinationForAction(const VSCKeyboard::Action& action) const
{
    ActionConstIterator it = mCombinationActionMap.right.find(action);
    
    if (it != mCombinationActionMap.right.end())
    {
        return it->second;
    }
    
    return VSCKeyboard::NullCombination;
}

bool VSCKeyBindings::eraseCombination(const VSCKeyboard::Combination& comb)
{
    mCombinationActionMap.left.erase(comb);
}

bool VSCKeyBindings::eraseAction(const VSCKeyboard::Action& action)
{
    mCombinationActionMap.right.erase(action);
}

void VSCKeyBindings::setBinding(const VSCKeyboard::Action& action, const VSCKeyboard::Combination& comb)
{
    
    /*
     *  Check if the combination is already bound
     */
    
    CombinationConstIterator combIt = mCombinationActionMap.left.find(comb);
    
    if (combIt != mCombinationActionMap.left.end())
    {
        throw VSCUIException("Attempted to set binding with already bound combination");
    }
    
    /*
     *  Check if the action is already bound
     */
    
    ActionConstIterator actionIt = mCombinationActionMap.right.find(action);
    
    if (actionIt != mCombinationActionMap.right.end())
    {
        throw VSCUIException("Attempted to set binding with already bound action");
    }
    
    mCombinationActionMap.insert(CombinationActionMapEntry(action, comb));
    
}




