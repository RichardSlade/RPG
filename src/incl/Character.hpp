#ifndef CHARACTER_HPP
#define CHARACTER_HPP

/*
*   Class for Character controlled by player
*
*   @Author Richard Slade
*   @Date 12/2014
*/

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

#include "MovingEntity.hpp"
#include "Path.hpp"
#include "StateMachine.hpp"

class Character : public MovingEntity
{
public:
    typedef std::vector<std::unique_ptr<State<Character>>> StateContainer;

    enum States
    {
        LookOut,
        Evade,
        Relax,
//        Exit,
        NumDogStates
    };

private:
    StateContainer&                 mStates;
    StateMachine<Character>         mStateMachine;

    virtual void                    updateCurrent(sf::Time);
    virtual void                    drawCurrent(sf::RenderTarget&
                                            , sf::RenderStates) const;

public:
                                    Character(Level*
                                        , const sf::Texture&
                                        , const sf::Font&
                                        , sf::Vector2f
                                        , EntityStats
                                        , const Params&
                                        , State<Character>*
                                        , State<Character>*
                                        , StateContainer&
                                        , float = 1.f);

    virtual                         ~Character(){};

    void                            addToPath(sf::Vector2f pos)
                                    { mSteering.addToPath(pos); }

    void                            startNewPath(sf::Vector2f pos)
                                    { mSteering.startNewPath(pos); }

    void                            changeState(Character::States newState)
                                    { mStateMachine.changeState(mStates.at(newState).get()); }
};

#endif // CHARACTER_HPP
