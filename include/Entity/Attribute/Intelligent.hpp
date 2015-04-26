#ifndef INTELLIGENT_HPP
#define INTELLIGENT_HPP

#include "Entity/EntityStats.hpp"

struct Intelligent
{
public:
   const float             PanicDistance;
   const float             AgroDistance;
//   const float             AttackDistance;

                           Intelligent(EntityStats stats)
                           : PanicDistance(stats.panicDistance)
                           , AgroDistance(stats.agroDistance)
//                           , AttackDistance(stats.attackDistance)
                           {};

};

#endif // INTELLIGENT_HPP
