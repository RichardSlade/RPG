/*
*  @Title meleeCombatant.cpp
*  @Author Richard Slade
*  @Date 19/04/2015
*
*/

#include "Entity/Attributes/Melee.hpp"
#include "Entity/Attributes/Killable.hpp"

MeleeCombatant::MeleeCombatant(sf::Time delay
                              , float baseDmg)
: mAttackDelay(delay)
, mBaseDmg(baseDmg)
, mAttackCountdown(delay)
, mCanAttack(true)
{}

void MeleeCombatant::update(sf::Time dt)
{
   if(!mCanAttack)
      mAttackCountdown -= dt;

   if(mAttackCountdown < sf::Time::Zero)
      mCanAttack = true;
}

void MeleeCombatant::meleeAttack(Killable* target)
{
   float dmg = -mBaseDmg;

   target->changeHealth(dmg);

   mAttackCountdown = mAttackDelay;
   mCanAttack = false;
}
