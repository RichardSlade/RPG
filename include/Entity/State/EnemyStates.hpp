#ifndef ENEMYSTATES_HPP
#define ENEMYSTATES_HPP

#include "Entity/State/State.hpp"
//#include "Entity/Enemy.hpp"

class Enemy;

namespace EnemyStates
{

struct LookOut : public State<Enemy>
{
public:
    virtual             ~LookOut() {};

    virtual void        enter(Enemy* host);

    virtual void        execute(Enemy* host);

    virtual void        exit(Enemy* host);
};

struct Evade : public State<Enemy>
{
public:
    virtual             ~Evade(){};

    virtual void        enter(Enemy* host);

    virtual void        execute(Enemy* host);

    virtual void        exit(Enemy* host);
};

struct Relax : public State<Enemy>
{
public:
    virtual             ~Relax() {};

    virtual void        enter(Enemy* host);

    virtual void        execute(Enemy* host);

    virtual void        exit(Enemy* host);
};

struct Attack : public State<Enemy>
{
public:
    virtual             ~Attack(){};

    virtual void        enter(Enemy* host);

    virtual void        execute(Enemy* host);

    virtual void        exit(Enemy* host);
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

#endif // ENEMYSTATES_HPP
