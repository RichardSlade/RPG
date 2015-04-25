#ifndef STATE_HPP
#define STATE_HPP

#include <memory>

template<class entity_type>
struct State
{
public:
    virtual ~State(){}

    //this will execute when the state is entered
    virtual void enter(entity_type*)=0;

    //this is the states normal update function
    virtual void execute(entity_type*)=0;

    //this will execute when the state is exited
    virtual void exit(entity_type*)=0;
};

#endif // STATE_HPP
