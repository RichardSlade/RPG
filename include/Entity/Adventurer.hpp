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

#include "Entity/Entity.hpp"
#include "Entity/Path.hpp"
#include "Entity/State/StateMachine.hpp"

class Enemy;

class Adventurer : public Entity
{
public:
    typedef std::vector<std::unique_ptr<State<Adventurer>>> StateContainer;

    enum States
    {
        LookOut,
        Evade,
        Relax,
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
    StateContainer&                 mStates;
    StateMachine<Adventurer>         mStateMachine;

//    Entity*                          mCurrentTarget;

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
                                        , State<Adventurer>* globalState
                                        , State<Adventurer>* initState
                                        , StateContainer&
                                        , unsigned int currentState
                                        , float = 1.f);

    virtual                         ~Adventurer(){};

    void                            addToPath(sf::Vector2f pos)
                                    { mSteering.addToPath(pos); }

    void                            startNewPath(sf::Vector2f pos)
                                    { mSteering.startNewPath(pos); }

    void                            changeState(Adventurer::States newState)
                                    { mStateMachine.changeState(mStates.at(newState).get(), newState); }

   unsigned int                     getCurrentStateType() {return mStateMachine.getCurrentStateType(); }

//   Entity*                           getCurrentTarget() {return mCurrentTarget;}
};

#endif // ADVENTURER_HPP
