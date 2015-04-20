#ifndef GOBLINSTATES_HPP
#define GOBLINSTATES_HPP

#include <Entity/State/State.hpp>
#include <Entity/Goblin.hpp>

namespace GoblinStates
{

struct LookOut : public State<Goblin>
{
public:
    virtual             ~LookOut() {};

    virtual void        enter(Goblin* host);

    virtual void        execute(Goblin* host);

    virtual void        exit(Goblin* host);
};

struct Evade : public State<Goblin>
{
public:
    virtual             ~Evade(){};

    virtual void        enter(Goblin* host);

    virtual void        execute(Goblin* host);

    virtual void        exit(Goblin* host);
};

struct Relax : public State<Goblin>
{
public:
    virtual             ~Relax() {};

    virtual void        enter(Goblin* host);

    virtual void        execute(Goblin* host);

    virtual void        exit(Goblin* host);
};

struct Attack : public State<Goblin>
{
public:
    virtual             ~Attack(){};

    virtual void        enter(Goblin* host);

    virtual void        execute(Goblin* host);

    virtual void        exit(Goblin* host);
};

//struct Exit : public State<Enemy>
//{
//public:
//    virtual             ~Exit(){};
//
//    virtual void        enter(Enemy* host);
//
//    virtual void        execute(Enemy* host);
//
//    virtual void        exit(Enemy* host);
//};

}

#endif // GOBLINSTATES_HPP
