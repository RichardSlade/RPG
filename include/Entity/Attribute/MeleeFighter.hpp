#ifndef MELEE_HPP
#define MELEE_HPP

#include <SFML/System/Time.hpp>

class MovingEntity;

class Melee
{
protected:
//   MovingEntity*     mHost;

   const sf::Time    mAttackDelay;
   const float       mBaseDmg;

   sf::Time          mAttackCountdown;
   bool              mCanAttack;

public:
                     Melee(sf::Time delay
                           , float baseDmg);

   void              update(sf::Time dt);
   void              meleeAttack(Killable* target);

   // Getters
   bool              canAttack() {return mCanAttack;}

};

#endif // MELEECOMBATANT_HPP
