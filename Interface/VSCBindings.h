
#ifndef _VSC_BINDINGS_H_
#define _VSC_BINDINGS_H_

#include "VSCUI.h"

#include <map>
#include <set>

#include <boost/shared_ptr.hpp>


/*
 *  VSCBindings stores and manipulates a group of bindings between Action and 
 *  Input types
 */

template<typename Action, typename Input>
class VSCBindings 
{
    
public:
    
    typedef boost::shared_ptr< VSCBindings<Action,Input> > SPtr;
    
    typedef std::set<Input>                     InputSet;
    typedef std::set<Action>                    ActionSet;
    
    VSCBindings() {}
    virtual ~VSCBindings() {}
    
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

//#include "VSCBindings.cpp"


#endif//_VSC_BINDINGS_H_

