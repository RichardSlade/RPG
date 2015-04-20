#ifndef INTELLIGENT_HPP
#define INTELLIGENT_HPP

#include "Entity/EntityStats.hpp"

struct Intelligent
{
public:
   const float             mPanicDistance;
   const float             mAgroDistance;
   const float             mAttackDistance;

                           Intelligent(EntityStats stats)
                           : mPanicDistance(stats.panicDist)
                           , mAgroDistance(stats.agroDist)
                           , mAttackDistance(stats.attackDist)
                           {};

};

#endif // INTELLIGENT_HPP
