#ifndef KILLABLE_HPP
#define KILLABLE_HPP

#include "Entity/EntityStats.hpp"

struct Killable
{
protected:
   const float                MaxHealth;
   float                      health;
   bool                       dead;

public:
                              Killable(float hp)
                              : MaxHealth(hp)
                              , health(MaxHealth)
                              , dead(false)
                              {};

   float                      getHealthPercentage() const {return (health / MaxHealth) * 100.f;}

   void                       changeHealth(float hpChange)
                              {
                                 health += hpChange;

                                 if(health <  0.f)
                                 {
                                    health = 0;
                                    dead = true;
                                 }
                              }

   bool                       isDead() const { return dead; }
};

#endif // KILLABLE_HPP
