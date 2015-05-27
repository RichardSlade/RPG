#ifndef INTELLIGENT_HPP
#define INTELLIGENT_HPP

#include "Entity/EntityStats.hpp"

struct Intelligent
{
public:
   const float             PanicDistance;
   const float             AggroDistance;
   const float             LoseAggroDistance;
//   const float             AttackDistance;

                           Intelligent(EntityStats stats)
                           : PanicDistance(stats.panicDistance)
                           , AggroDistance(stats.aggroDistance)
                           , LoseAggroDistance(stats.loseAggroDistance)
//                           , AttackDistance(stats.attackDistance)
                           {};

};

#endif // INTELLIGENT_HPP
