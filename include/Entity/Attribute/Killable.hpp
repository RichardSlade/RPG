#ifndef KILLABLE_HPP
#define KILLABLE_HPP

#include "Entity/EntityStats.hpp"

struct Killable
{
protected:
   const float                mMaxHealth;
   float                      mHealth;

public:
   float                      getHealthPercentage() const {return (mHealth / mMaxHealth) * 100.f;}
   void                       changeHealth(float hpChange) {mHealth += hpChange;}

                              Killable(float hp)
                              : mMaxHealth(hp)
                              , mHealth(mMaxHealth)
                              {};
};

#endif // KILLABLE_HPP
