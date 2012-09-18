
#ifndef _VSC_BINDINGS_H_
#define _VSC_BINDINGS_H_

#include <map>
#include <set>

#include <boost/shared_ptr.hpp>
#include <boost/foreach.hpp>

/*
 *  VSC::Bindings stores and manipulates a group of bindings between Action and 
 *  Input types
 */

template<typename Action, typename Input>
class VSC::Bindings 
{
    
public:
    
    typedef typename boost::shared_ptr< VSC::Bindings<Action,Input> > SPtr;
    
    typedef typename std::set<Input>                     InputSet;
    typedef typename std::set<Action>                    ActionSet;
    
    VSC::Bindings() {}
    virtual ~VSC::Bindings() {}
    
    /*
     *  Get the Action/Combination associated with a Combination/Action. Returns NullAction/NullCombination
     *  if none was found.
     */
    
    const ActionSet& getActionsForInput(const Input& input);
    const InputSet& getInputsForAction(const Action& action);
    
    /*
     *  Erase the binding for a given Action or Combination. 
     */
    
    void eraseBinding(const Action& action, const Input& input);
    void eraseInputBindings(const Input& input);
    void eraseActionBindings(const Action& action);
    
    /*
     *  Set a binding, throws an exception if the action and/or the combination is already bound.
     */
    
    void setBinding(const Action& action, const Input& input);
    
private:
    
    typedef std::map<Action, InputSet>          ActionInputsMap;
    typedef std::map<Input, ActionSet>          InputActionsMap;
    
    ActionInputsMap   mActionInputsMap;
    InputActionsMap   mInputActionsMap;
    
};

template<typename Action, typename Input>
const typename VSC::Bindings<Action,Input>::ActionSet& VSC::Bindings<Action,Input>::getActionsForInput(const Input& input)
{
    typename InputActionsMap::iterator it = mInputActionsMap.find(input);
    
    if (it == mInputActionsMap.end())
    {
        /*
         *  If we don't have anything, then create an empty set and return that.
         *  NOTE: DON'T just create a local and return it as it would go out of
         *  scope and be destroyed (potentially) as soon as the function returns.
         */
        
        ActionSet actionSet;
        mInputActionsMap[input] = actionSet;
        
        it = mInputActionsMap.find(input);
    }
    
    return it->second;
}

template<typename Action, typename Input>
const typename VSC::Bindings<Action,Input>::InputSet& VSC::Bindings<Action,Input>::getInputsForAction(const Action& action)
{
    typename ActionInputsMap::iterator it = mActionInputsMap.find(action);
    
    if (it == mInputActionsMap.end())
    {
        /*
         *  If we don't have anything, then create an empty set and return that.
         *  NOTE: DON'T just create a local and return it as it would go out of
         *  scope and be destroyed (potentially) as soon as the function returns.
         */
        
        InputSet inputSet;
        mActionInputsMap[action] = inputSet;
        
        it = mActionInputsMap.find(action);
    }
    
    return it->second;
}

template<typename Action, typename Input>
void VSC::Bindings<Action,Input>::eraseBinding(const Action& action, const Input& input)
{
    {
        typename ActionInputsMap::iterator it = mActionInputsMap.find(action);
        
        if (it != mActionInputsMap.end())
        {
            it->second.erase(input);
        }
    }
    
    {
        typename InputActionsMap::iterator it = mInputActionsMap.find(input);
        
        if (it != mInputActionsMap.end())
        {
            it->second.erase(action);
        }
    }
}

template<typename Action, typename Input>
void VSC::Bindings<Action,Input>::eraseInputBindings(const Input& input)
{
    /*
     *  Have to cycle through all the input sets to remove the input. A bit expensive but
     *  not a time critical operation...
     */
    
    mInputActionsMap.erase(input);
    
    BOOST_FOREACH (typename ActionInputsMap::value_type& pair, mActionInputsMap)
    {
        pair->second.erase(input);
    }
}

template<typename Action, typename Input>
void VSC::Bindings<Action,Input>::eraseActionBindings(const Action& action)
{
    mActionInputsMap.erase(action);
    
    BOOST_FOREACH (typename InputActionsMap::value_type& pair, mInputActionsMap)
    {
        pair->second.erase(action);
    }
}

template<typename Action, typename Input>
void VSC::Bindings<Action,Input>::setBinding(const Action& action, const Input& input)
{
    /*
     *  Update input set for action
     */
    
    {
        typename ActionInputsMap::iterator it = mActionInputsMap.find(action);
        if (it == mActionInputsMap.end())
        {
            InputSet inputSet;
            mActionInputsMap[action] = inputSet;
            it = mActionInputsMap.find(action);
        }
        it->second.insert(input);
    }
    
    /*
     *  Update action set for input
     */
    
    {
        typename InputActionsMap::iterator it = mInputActionsMap.find(input);
        if (it == mInputActionsMap.end())
        {
            ActionSet actionSet;
            mInputActionsMap[input] = actionSet;
            it = mInputActionsMap.find(input);
        }
        it->second.insert(action);
    }
}

#endif//_VSC_BINDINGS_H_

