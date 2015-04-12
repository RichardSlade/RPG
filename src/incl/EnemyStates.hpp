#ifndef ENEMYSTATES_HPP
#define ENEMYSTATES_HPP

#include "State.hpp"
#include "Enemy.hpp"

struct LookOutForCharacter : public State<Enemy>
{
public:
    virtual             ~LookOutForCharacter(){};

    virtual void        enter(Enemy* enemy);

    virtual void        execute(Enemy* enemy);

    virtual void        exit(Enemy* enemy);
};

struct EvadeCharacter : public State<Enemy>
{
public:
    virtual             ~EvadeCharacter(){};

    virtual void        enter(Enemy* enemy);

    virtual void        execute(Enemy* enemy);

    virtual void        exit(Enemy* enemy);
};

struct RelaxEnemy : public State<Enemy>
{
public:
    virtual             ~RelaxEnemy(){};

    virtual void        enter(Enemy* enemy);

    virtual void        execute(Enemy* enemy);

    virtual void        exit(Enemy* enemy);
};

//struct Exit : public State<Enemy>
//{
//public:
//    virtual             ~Exit(){};
//
//    virtual void        enter(Enemy* enemy);
//
//    virtual void        execute(Enemy* enemy);
//
//    virtual void        exit(Enemy* enemy);
//};

#endif // ENEMYSTATES_HPP
