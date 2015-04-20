#ifndef ENEMY_HPP
#define ENEMY_HPP

//#include <vector>
#include <memory>

#include "Entity/MovingEntity.hpp"
#include "Entity/State/StateMachine.hpp"
#include "Entity/Attribute/MeleeFighter.hpp"
//#include "Character.hpp"

//class Character;

class DynamicEnemy : public MovingEntity,
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
   sf::Vector2i                mTargetBlockIndex;

   //    std::vector<State<Enemy>>&  mStates;
//   StateContainer&             mStates;
//   StateMachine<Enemy>         mStateMachine;


   virtual void               updateCurrent(sf::Time);
   virtual void               drawCurrent(sf::RenderTarget&
                                         , sf::RenderStates) const;

public:
                              Enemy(Level*
                                    , const sf::Texture&
                                    , const sf::Font&
                                    , sf::Vector2f
                                    , EntityStats
                                    , const Params&
//                                          , State<Enemy>*
//                                          , State<Enemy>*
//                                          , StateContainer&
//                                          , Enemy::StateType
                                    , float = 1.f);

   virtual                    ~Enemy() {};

//   int                           getCurrentState() {return mStateMachine.getCurrentStateType(); }
//
//   void                          changeState(Enemy::StateType newState){ mStateMachine.changeState(mStates.at(newState).get(), newState); }

   // Getters
   LevelBlock*                getTargetBlock() {return getLevelBlock(mTargetBlockIndex);}

   void                       setTargetBlockIndex(sf::Vector2i index) {mTargetBlockIndex = index;}
};

#endif // ENEMY_HPP
