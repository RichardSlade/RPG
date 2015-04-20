#ifndef ADVENTURER_HPP
#define ADVENTURER_HPP

/*
*   Class for Character controlled by player
*
*   @Author Richard Slade
*   @Date 12/2014
*/

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

#include "Entity/MovingEntity.hpp"
#include "Entity/Attribute/MeleeFighter.hpp"
#include "Entity/Path.hpp"
#include "Entity/State/StateMachine.hpp"

class Adventurer : public MovingEntity, public MeleeFighter
{
public:
   typedef std::vector<std::unique_ptr<State<Character>>> StateContainer;

   enum StateType
   {
     LookOut,
   //        Evade,
     Relax,
     Attack,
   //        Follow,
   //        Exit,
     NumStates
   };

protected:
   StateContainer&                 mStates;
   StateMachine<Character>         mStateMachine;

//   MeleeController                 mMelee;

   virtual void                    updateCurrent(sf::Time);
   virtual void                    drawCurrent(sf::RenderTarget&
                                             , sf::RenderStates) const;

public:
                                    Adventurer(Level*
                                             , const sf::Texture&
                                             , const sf::Font&
                                             , sf::Vector2f
                                             , EntityStats
                                             , const Params&
                                             , State<Character>*
                                             , State<Character>*
                                             , StateContainer&
                                             , Character::StateType
                                             , float = 1.f);

   virtual                          ~Adventurer() {};

//   void                             meleeAttack(MovingEntity*) {mMelee.attack(target)};

   // Getters
   int                              getCurrentState() {return mStateMachine.getCurrentStateType(); }

   // Setters
   void                             addToPath(sf::Vector2f pos) {mSteering.addToPath(pos);}

   void                             startNewPath(sf::Vector2f pos) {mSteering.startNewPath(pos);}

   void                             changeState(Character::StateType newState)
                                    {
                                       mStateMachine.changeState(mStates.at(newState).get(), newState);
                                    }
};

#endif // ADVENTURER_HPP
