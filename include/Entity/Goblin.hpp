#ifndef GOBLIN_HPP
#define GOBLIN_HPP

#include "Entity/Enemy.hpp"
#include "Entity/Attribute/Killable.hpp"
#include "Entity/Attribute/Intelligent.hpp"
#include "Entity/Attribute/MeleeFighter.hpp"

class Goblin : public Enemy, public Killable, public Intelligent, public Melee
{
public:
   typedef std::vector<std::unique_ptr<State<Goblin>>> StateContainer;

   enum StateType
   {
      LookOut,
      Relax,
      Evade,
      Attack,
      //        Exit,
      NumStates
   };

private:
   StateContainer&               mStates;
   StateMachine<Goblin>          mStateMachine;

public:
                                 Goblin(Level*
                                    , const sf::Texture&
                                    , const sf::Font&
                                    , sf::Vector2f
                                    , EntityStats
                                    , const Params& params
                                    , State<Enemy>* globalState
                                    , State<Enemy>* initState
                                    , StateContainer& states
                                    , Enemy::StateType currentStateType
                                    , float = 1.f)
                                 : Enemy(mLevel.get()
                                         , controller.getTexture(Controller::Textures::Enemy)
                                         , controller.getFont(Controller::Fonts::Sansation)
                                         , pos
                                         , mEntityStats.at(StatsType::EnemyStats)
                                         , controller.getParams()
                                         , rangedClamped(0.75f, 1.25f))
                                 , Melee(sf::seconds(stats.attackDelay), stats.damage)
                                 , mStates(states)
                                 , mStateMachine(this
                                                , globalState
                                                , initState
                                                , currentStateType);

   virtual                       ~Goblin(){};

   void                          update(sf::Time dt);

   int                           getCurrentState() {return mStateMachine.getCurrentStateType(); }

   void                          changeState(Enemy::StateType newState){ mStateMachine.changeState(mStates.at(newState).get(), newState); }

};

#endif // GOBLIN_HPP
