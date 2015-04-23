#ifndef ENEMY_HPP
#define ENEMY_HPP

#include "Entity/Entity.hpp"
#include "Entity/State/StateMachine.hpp"
#include "Entity/State/EnemyStates.hpp"
#include "Entity/Attribute/Intelligent.hpp"
#include "Entity/Attribute/Killable.hpp"
#include "Entity/Attribute/MeleeFighter.hpp"

class Enemy : public Entity, public Intelligent, public Killable,  public MeleeFighter
{
public:
   typedef std::vector<std::unique_ptr<State<Enemy>>> StateContainer;

   enum StateType
   {
      LookOut,
      Relax,
      Evade,
      Attack,
      //        Exit,
      NumStates
   };

   enum Type
   {
      Goblin,
      NumEnemyTypes
   };

protected:
   Enemy::Type             mEnemyType;

   StateContainer&         mStates;
   StateMachine<Enemy>    mStateMachine;

   virtual void            updateCurrent(sf::Time dt);
   virtual void            drawCurrent(sf::RenderTarget& target
                                       , sf::RenderStates states) const;

public:

                           Enemy(Enemy::Type enemyType
                                 , const sf::Texture& texture
                                 , const sf::Font& font
                                 , sf::Vector2f startPos
                                 , float scale
                                 , const EntityStats& stats
                                 , const Params& params
                                 , Level* level
                                 , State<Enemy>* globalState
                                 , State<Enemy>* initState
                                 , StateContainer& states
                                 , Enemy::StateType currentStateType);

   virtual                 ~Enemy(){};


   void                    changeState(Enemy::StateType newState) { mStateMachine.changeState(mStates.at(newState).get(), newState);}

   // Getters
   int                     getEnemyType() {return mEnemyType;}

   int                     getCurrentStateType() {return mStateMachine.getCurrentStateType();}
};

#endif // ENEMY_HPP

