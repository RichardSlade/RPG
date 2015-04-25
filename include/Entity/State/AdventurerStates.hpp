#ifndef ADVENTURERSTATES_HPP
#define ADVENTURERSTATES_HPP

<<<<<<< HEAD
#include <Entity/State/State.hpp>
#include <Entity/Adventurer.hpp>
=======
#include "Entity/State/State.hpp"
#include "Entity/Adventurer.hpp"
>>>>>>> working

namespace AdventurerStates
{

struct LookOut : public State<Adventurer>
{
public:
<<<<<<< HEAD
    virtual             ~LookOut() {};
=======
    virtual             ~LookOut(){};
>>>>>>> working

    virtual void        enter(Adventurer* host);

    virtual void        execute(Adventurer* host);

    virtual void        exit(Adventurer* host);
};

<<<<<<< HEAD
//struct Evade : public State<Adventurer>
//{
//public:
//    virtual             ~Evade() {};
//
//    virtual void        enter(Adventurer* host);
//
//    virtual void        execute(Adventurer* host);
//
//    virtual void        exit(Adventurer* host);
//};

struct Relax : public State<Adventurer>
{
public:
    virtual             ~Relax() {};
=======
struct Evade : public State<Adventurer>
{
public:
    virtual             ~Evade(){};
>>>>>>> working

    virtual void        enter(Adventurer* host);

    virtual void        execute(Adventurer* host);

    virtual void        exit(Adventurer* host);
};

<<<<<<< HEAD
struct Attack : public State<Adventurer>
{
public:
    virtual             ~Attack() {};
=======
struct Relax : public State<Adventurer>
{
public:
    virtual             ~Relax(){};
>>>>>>> working

    virtual void        enter(Adventurer* host);

    virtual void        execute(Adventurer* host);

    virtual void        exit(Adventurer* host);
};

<<<<<<< HEAD
//struct Follow : public State<Adventurer>
//{
//public:
//    virtual            ~Follow() {};
=======
//struct FollowCharacter : public State<Adventurer>
//{
//public:
//    virtual            ~FollowCharacter(){};
>>>>>>> working
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
<<<<<<< HEAD

=======
>>>>>>> working
}

#endif // ADVENTURERSTATES_HPP
