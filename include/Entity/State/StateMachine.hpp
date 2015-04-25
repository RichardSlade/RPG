#ifndef STATEMACHINE_HPP
#define STATEMACHINE_HPP

#include <cassert>

#include <Entity/State/State.hpp>

template<class entity_type>
class StateMachine
{
private:
    entity_type*                mHost;
    State<entity_type>*         mGlobalState;
    State<entity_type>*         mCurrentState;
    State<entity_type>*         mPreviousState;

    int                         mCurrentStateType;

public:
    StateMachine(entity_type* host
                 , State<entity_type>* globalState
                 , State<entity_type>* initState
                 , int currentStateType)
	  : mHost(host)
	  , mGlobalState(globalState)
	  , mCurrentState(initState)
	  , mPreviousState(initState)
	  , mCurrentStateType(currentStateType)
    {
        mGlobalState->enter(mHost);
        mCurrentState->enter(mHost);
    };

    StateMachine() {};

	void                       update()
										{
										  if(mGlobalState)
												mGlobalState->execute(mHost);

										  if(mCurrentState)
												mCurrentState->execute(mHost);
										}

    // Setters
   void                       changeState(State<entity_type>* newState, int stateType)
                              {
                                 assert(newState
                                        && "StateMachine::changeState attempting to change to null state");

                                 mCurrentStateType = stateType;

                                 if(mCurrentState)
                                 {
                                 mPreviousState = mCurrentState;
                                 mCurrentState->exit(mHost);
                                 }

                                 mCurrentState = newState;
                                 mCurrentState->enter(mHost);
                              }

	void								changeGlobalState(State<entity_type>* newState)
										{
											assert(newState
													&& "StateMachine::changeGlobalState attempting to change to null state");

											if(mGlobalState)
												mGlobalState->exit(mHost);


											mGlobalState = newState;
											mGlobalState->enter(mHost);
										}

	void								setGlobalState(State<entity_type>* newState) {mGlobalState = newState;}

	void                       returnToPreviousState() {changeState(mPreviousState);}

   int                        getCurrentStateType() {return mCurrentStateType;}

};

#endif // STATEMACHINE_HPP
