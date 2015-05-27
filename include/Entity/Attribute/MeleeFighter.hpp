#ifndef MELEEFIGHTER_HPP
#define MELEEFIGHTER_HPP

#include <SFML/System/Time.hpp>

#include "Entity/EntityStats.hpp"
//#include "Entity/Attribute/Killable.hpp"

//class MovingEntity;
class Killable;

class MeleeFighter
{

//   MovingEntity*     mHost;
public:
   const sf::Time    mAttackDelay;
   const float       mBaseDmg;
   const float       mAttackDistance;

protected:
   sf::Time          mAttackCountdown;
   bool              mCanAttack;

public:
                     MeleeFighter(EntityStats stats);

   void              update(sf::Time dt);
   void              meleeAttack(Killable* target);

   // Getters
   bool              canAttack() {return mCanAttack;}

};

#endif // MELEEFIGHTER_HPP
