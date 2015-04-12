#ifndef CHARACTERSTATES_HPP
#define CHARACTERSTATES_HPP

#include "State.hpp"
#include "Character.hpp"

struct LookOutForEnemy : public State<Character>
{
public:
    virtual             ~LookOutForEnemy(){};

    virtual void        enter(Character* character);

    virtual void        execute(Character* character);

    virtual void        exit(Character* character);
};

struct EvadeEnemy : public State<Character>
{
public:
    virtual             ~EvadeEnemy(){};

    virtual void        enter(Character* character);

    virtual void        execute(Character* character);

    virtual void        exit(Character* character);
};

struct RelaxChar : public State<Character>
{
public:
    virtual             ~RelaxChar(){};

    virtual void        enter(Character* character);

    virtual void        execute(Character* character);

    virtual void        exit(Character* character);
};

struct FollowCharacter : public State<Character>
{
public:
    virtual            ~FollowCharacter(){};

    virtual void        enter(Character* character);

    virtual void        execute(Character* character);

    virtual void        exit(Character* character);
};


//struct Exit : public State<Character>
//{
//public:
//    virtual             ~Exit(){};
//
//    virtual void        enter(Character* character);
//
//    virtual void        execute(Character* character);
//
//    virtual void        exit(Character* character);
//};


#endif // CHARACTERSTATES_HPP
