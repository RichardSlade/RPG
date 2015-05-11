#ifndef APPSTATE_HPP
#define APPSTATE_HPP

/*
*   Base class for App States
*
*   @Author Richard Slade
*   @Date 12/2014
*/

#include <SFML/System/Time.hpp>

struct AppState
{
public:
    enum StateType
    {
        Menu,
        Game,
        NumStateTypes
    };

    virtual             ~AppState(){};

    virtual void        lockedUpdate(sf::Time) = 0;
    virtual void        unlockedUpdate() = 0;
    virtual void        handleInput() = 0;
    virtual void        display() = 0;
};

#endif // APPSTATE_HPP
