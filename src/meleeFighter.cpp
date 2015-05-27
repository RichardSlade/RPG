/*
*  @Title meleeCombatant.cpp
*  @Author Richard Slade
*  @Date 19/04/2015
*
*/

#include "Entity/Attribute/MeleeFighter.hpp"
#include "Entity/Attribute/Killable.hpp"

MeleeFighter::MeleeFighter(EntityStats stats)
: mAttackDelay(sf::seconds(stats.attackDelay))
, mBaseDmg(stats.baseDamage)
, mAttackDistance(stats.attackDistance)
, mAttackCountdown(mAttackDelay)
, mCanAttack(true)
{}

void MeleeFighter::update(sf::Time dt)
{
   if(!mCanAttack)
      mAttackCountdown -= dt;

   if(mAttackCountdown < sf::Time::Zero)
      mCanAttack = true;
}

void MeleeFighter::meleeAttack(Killable* target)
{
   if(mCanAttack)
   {
      float dmg = -mBaseDmg;

      target->changeHealth(dmg);

      mAttackCountdown = mAttackDelay;
      mCanAttack = false;
   }
}
