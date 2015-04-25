#ifndef ENEMYSTATES_HPP
#define ENEMYSTATES_HPP

#include "Entity/State/State.hpp"
<<<<<<< HEAD
//#include "Entity/Enemy.hpp"

class Enemy;
=======
#include "Entity/Enemy.hpp"
>>>>>>> working

namespace EnemyStates
{

struct LookOut : public State<Enemy>
{
public:
<<<<<<< HEAD
    virtual             ~LookOut() {};
=======
    virtual             ~LookOut(){};
>>>>>>> working

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
<<<<<<< HEAD
    virtual             ~Relax() {};

    virtual void        enter(Enemy* host);

    virtual void        execute(Enemy* host);

    virtual void        exit(Enemy* host);
};

struct Attack : public State<Enemy>
{
public:
    virtual             ~Attack(){};
=======
    virtual             ~Relax(){};
>>>>>>> working

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
<<<<<<< HEAD

=======
>>>>>>> working
}

#endif // ENEMYSTATES_HPP
