#ifndef GOBLIN_HPP
#define GOBLIN_HPP

#include "Entity/Enemy.hpp"
//#include "Entity/Attribute/Killable.hpp"
//#include "Entity/Attribute/Intelligent.hpp"
#include "Entity/Enemy.hpp"
#include "Entity/Attribute/MeleeFighter.hpp"
#include "Entity/State/State.hpp"
#include "Entity/State/StateMachine.hpp"

class Goblin : public Enemy, public MeleeFighter
{
//public:
//   typedef std::vector<std::unique_ptr<State<Goblin>>> StateContainer;
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

private:
//   StateContainer&               mStates;
//   StateMachine<Goblin>          mStateMachine;

   virtual void                  updateCurrent(sf::Time dt)
                                 {
                                    Enemy::updateCurrent(dt);
                                    MeleeFighter::update(dt);
                                 }


public:
                                 Goblin(Level* level
                                             , const sf::Texture& texture
                                             , const sf::Font& font
                                             , sf::Vector2f startPos
                                             , float scale
                                             , EntityStats stats
                                             , const Params& params
                                             , State<Enemy>* globalState
                                             , State<Enemy>* initState
                                             , StateContainer& states
                                             , Goblin::StateType currentStateType)
                                 : Enemy(Enemy::EnemyType::Goblin
                                       , level
                                       , texture
                                       , font
                                       , startPos
                                       , scale
                                       , stats
                                       , params
                                       , globalState
                                       , initState
                                       , states
                                       , currentStateType)
                                 , MeleeFighter(sf::seconds(stats.attackDelay)
                                                 , stats.meleeDamage
                                                 , stats.attackDist)
                                 {};

   virtual                       ~Goblin(){};

//   int                           getCurrentState() {return mStateMachine.getCurrentStateType(); }

//   void                          changeState(Goblin::StateType newState){ mStateMachine.changeState(mStates.at(newState).get(), newState); }

};

#endif // GOBLIN_HPP
