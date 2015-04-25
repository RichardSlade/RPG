#ifndef ADVENTURERSTATES_HPP
#define ADVENTURERSTATES_HPP

#include "Entity/State/State.hpp"
#include "Entity/Adventurer.hpp"

namespace AdventurerStates
{

struct LookOut : public State<Adventurer>
{
public:
    virtual             ~LookOut(){};

    virtual void        enter(Adventurer* host);

    virtual void        execute(Adventurer* host);

    virtual void        exit(Adventurer* host);
};

struct Evade : public State<Adventurer>
{
public:
    virtual             ~Evade(){};

    virtual void        enter(Adventurer* host);

    virtual void        execute(Adventurer* host);

    virtual void        exit(Adventurer* host);
};

struct Relax : public State<Adventurer>
{
public:
    virtual             ~Relax(){};

    virtual void        enter(Adventurer* host);

    virtual void        execute(Adventurer* host);

    virtual void        exit(Adventurer* host);
};

//struct FollowCharacter : public State<Adventurer>
//{
//public:
//    virtual            ~FollowCharacter(){};
//
//    virtual void        enter(Adventurer* host);
//
//    virtual void        execute(Adventurer* host);
//
//    virtual void        exit(Adventurer* host);
//};


//struct Exit : public State<Adventurer>
//{
//public:
//    virtual             ~Exit(){};
//
//    virtual void        enter(Adventurer* host);
//
//    virtual void        execute(Adventurer* host);
//
//    virtual void        exit(Adventurer* host);
//};
}

#endif // ADVENTURERSTATES_HPP
