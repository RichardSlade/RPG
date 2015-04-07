#ifndef DOG_HPP
#define DOG_HPP

/*
*   Class for sheep dog controlled by player
*
*   @Author Richard Slade
*   @Date 12/2014
*/

#include <SFML/Graphics/CircleShape.hpp>
#include <SFML/Graphics/Texture.hpp>
#include <SFML/System/Time.hpp>

#include "MovingEntity.hpp"
#include "Path.hpp"

class Dog : public MovingEntity
{
public:
    enum States
    {
        LookOut,
        Waypoints,
        DogRelax,
        NumDogStates
    };

private:
    virtual void                    updateCurrent(sf::Time);
    virtual void                    drawCurrent(sf::RenderTarget&
                                            , sf::RenderStates) const;

public:
                                    Dog(World*
                                        , const sf::Texture&
                                        , const sf::Font&
                                        , sf::Vector2f
                                        , EntityStats
                                        , const Params&);

    virtual                         ~Dog(){};

    void                            addToPath(sf::Vector2f pos)
                                    { mSteering.addToPath(pos); }

    void                            startNewPath(sf::Vector2f pos)
                                    { mSteering.startNewPath(pos); }
};

#endif // DOG_HPP
