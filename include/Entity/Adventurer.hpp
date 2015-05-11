#ifndef ADVENTURER_HPP
#define ADVENTURER_HPP

/*
*   Class for Adventurer controlled by player
*
*   @Author Richard Slade
*   @Date 12/2014
*/

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Window.hpp>

#include <Box2D/Box2D.h>

#include "Entity/Entity.hpp"
#include "Entity/Path.hpp"
#include "Entity/State/StateMachine.hpp"
//#include "Physics/DynamicPhysicsBody.hpp"

class Enemy;

class Adventurer : public Entity
{
public:
   typedef std::vector<std::unique_ptr<State<Adventurer>>> StateContainer;

   enum States
   {
      LookOut,
      Relax,
//      Evade,
      Attack,
      //        Exit,
      NumStates
   };

   enum Type
   {
      Barbarian,
      NumTypes
   };

private:
    const sf::RenderWindow&        mWindow;
    StateContainer&                mStates;
    StateMachine<Adventurer>       mStateMachine;

    b2Body*                        mPhysicsBody;

    bool                           mIsSelected;
    bool                           mMove;

//    Entity*                          mCurrentTarget;

    virtual void                    updateCurrent(sf::Time);
    virtual void                    drawCurrent(sf::RenderTarget&
                                            , sf::RenderStates) const;

public:
                                    Adventurer(const sf::RenderWindow& window
                                              , Level*
                                              , const sf::Texture&
                                              , const sf::Font&
                                              , sf::Vector2f
                                              , EntityStats
                                              , const Params&
                                              , State<Adventurer>* globalState
                                              , State<Adventurer>* initState
                                              , StateContainer&
                                              , unsigned int currentState
                                              , float physicsWorldScale
                                              , b2Body* body
                                              , float scale = 1.f);

   virtual                          ~Adventurer(){};

   void                             rotateToCursor();

   void                             addToPath(sf::Vector2f pos)
                                    { mSteering.addToPath(pos); }

   void                             startNewPath(sf::Vector2f pos)
                                    { mSteering.startNewPath(pos); }

   // Getters
   unsigned int                     getCurrentStateType() {return mStateMachine.getCurrentStateType(); }

   bool                             isSelected(){ return mIsSelected; }

   // Setters
   void                             changeState(Adventurer::States newState)
                                    { mStateMachine.changeState(mStates.at(newState).get(), newState); }

   void                             setIsSelected(bool status){ mIsSelected = status; }

   void                             setMove() { mMove = true; }

//   Entity*                           getCurrentTarget() {return mCurrentTarget;}
};

#endif // ADVENTURER_HPP
