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

//#include "Entity/Enemy.hpp"
#include "Entity/Entity.hpp"
//#include "Entity/Agent.hpp"
#include "Entity/Attribute/Intelligent.hpp"
#include "Entity/Attribute/Killable.hpp"
#include "Entity/Attribute/MeleeFighter.hpp"
//#include "Entity/Path.hpp"
#include "Entity/State/StateMachine.hpp"

//class Enemy;

class Adventurer : public Entity, public Intelligent, public Killable, public MeleeFighter
{
public:
   typedef std::vector<std::unique_ptr<State<Adventurer>>> StateContainer;

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

   enum Type
   {
      Barbarian,
      NumAdventurerTypes
   };

protected:
   Adventurer::Type              mAdventurerType;

   StateContainer&               mStates;
   StateMachine<Adventurer>         mStateMachine;

//   Enemy                           mCurrentTarget;

//   MeleeController                 mMelee;

   virtual void                    updateCurrent(sf::Time);
   virtual void                    drawCurrent(sf::RenderTarget&
                                             , sf::RenderStates) const;

public:
//                                    Adventurer(Level*
//                                             , const sf::Texture&
//                                             , const sf::Font&
//                                             , sf::Vector2f
//                                             , EntityStats
//                                             , const Params&
//                                             , State<Adventurer>*
//                                             , State<Adventurer>*
//                                             , StateContainer&
//                                             , Adventurer::StateType
//                                             , float = 1.f);

                                    Adventurer(Adventurer::Type adventurerType
                                             , const sf::Texture& texture
                                             , const sf::Font& font
                                             , sf::Vector2f startPos
                                             , float scale
                                             , EntityStats stats
                                             , const Params& params
                                             , Level* level
                                             , State<Adventurer>* globalState
                                             , State<Adventurer>* initState
                                             , StateContainer& states
                                             , Adventurer::StateType currentStateType);

   virtual                          ~Adventurer() {};

      void                    changeState(Adventurer::StateType newState) { mStateMachine.changeState(mStates.at(newState).get(), newState);}


//   void                             meleeAttack(MovingEntity*) {mMelee.attack(target)};

   // Getters
   int                              getAdventurerType() {return mAdventurerType;}

   int                              getCurrentState() {return mStateMachine.getCurrentStateType(); }

//   Enemy*                           getCurrentTarget() {return mCurrentTarget;}

   // Setters
   void                             addToPath(sf::Vector2f pos) {mSteering.addToPath(pos);}

   void                             startNewPath(sf::Vector2f pos) {mSteering.startNewPath(pos);}

//   void                             changeState(Adventurer::StateType newState)
//                                    {
//                                       mStateMachine.changeState(mStates.at(newState).get(), newState);
//                                    }
};

#endif // ADVENTURER_HPP
