#ifndef AGENT_HPP
#define AGENT_HPP

//#include <vector>
#include <memory>

#include <SFML/Graphics/Text.hpp>

#include "App/Params.hpp"
//#include "Entity/DynamicEntity.hpp"
#include "Entity/Attribute/Killable.hpp"
#include "Entity/Attribute/Intelligent.hpp"
//#include "Entity/State/StateMachine.hpp"
//#include "Entity/Attribute/MeleeFighter.hpp"
//#include "Entity/Adventurer.hpp"

//class Character;
//class Adventurer;

//class Params;

class Agent: public Killable, public Intelligent
{
public:
//   typedef std::vector<std::unique_ptr<State<Enemy>>> StateContainer;
//
//   enum StateType
//   {
//      LookOut,
//      Relax,
//      Evade,
//      Attack,
//      //        Exit,
//      NumStates
//   };

   const float                 mSightRange;
   const float                 mAngleOfVision;

protected:

//   Adventurer*                 mCurrentTarget;

   //    std::vector<State<Enemy>>&  mStates;
//   StateContainer&             mStates;
//   StateMachine<Enemy>         mStateMachine;




//   virtual void               updateCurrent(sf::Time);
//   virtual void               drawCurrent(sf::RenderTarget&
//                                         , sf::RenderStates) const;

public:
                              Agent(const sf::Font& font
                                          , EntityStats stats
                                          , const Params& params)
                              : Killable(stats.health)
                              , Intelligent(stats)
                              //, MeleeCombatant(dynamic_cast<MovingEntity*>(this)
                              //                  , sf::seconds(stats.attackDelay))
                              //, mCurrentTarget(nullptr)
                              //, mPanicDistance(params.AgentPanicDistance)
                              //, mStates(states)
                              //, mStateMachine(this
                              //               , globalState
                              //               , initState
                              //               , currentStateType)
                              {
//                                  mText.setPosition(-10.f, -40.f);
                              }



//   virtual                    ~Agent() {};

//   int                           getCurrentState() {return mStateMachine.getCurrentStateType(); }
//
//   void                          changeState(Enemy::StateType newState){ mStateMachine.changeState(mStates.at(newState).get(), newState); }

   // Getters
//   LevelBlock*                getTargetBlock() {return getLevelBlock(mTargetBlockIndex);}

//   Adventurer*                getCurrentTarget() {return mCurrentTarget;}

//   void                       setTargetBlockIndex(sf::Vector2i index) {mTargetBlockIndex = index;}
};

#endif // AGENT_HPP
