#ifndef INTELLIGENT_HPP
#define INTELLIGENT_HPP

#include "Entity/EntityStats.hpp"

struct Intelligent
{
public:
   const float             panicDistance;
   const float             agroDistance;
   const float             attackDistance;

                           Intelligent(EntityStats stats)
                           : panicDistance(stats.panicDistance)
                           , agroDistance(stats.agroDistance)
                           , attackDistance(stats.attackDistance)
                           {};

};

#endif // INTELLIGENT_HPP
