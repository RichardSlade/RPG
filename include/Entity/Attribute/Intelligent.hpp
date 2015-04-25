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
                           : panicDistance(stats.panicDist)
                           , agroDistance(stats.agroDist)
                           , attackDistance(stats.attackDist)
                           {};

};

#endif // INTELLIGENT_HPP
