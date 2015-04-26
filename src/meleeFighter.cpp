/*
*  @Title meleeCombatant.cpp
*  @Author Richard Slade
*  @Date 19/04/2015
*
*/

#include "Entity/Attribute/MeleeFighter.hpp"
#include "Entity/Attribute/Killable.hpp"

MeleeFighter::MeleeFighter(EntityStats stats)
: AttackDelay(sf::seconds(stats.attackDelay))
, BaseDmg(stats.baseDamage)
, AttackDistance(stats.attackDistance)
, mAttackCountdown(AttackDelay)
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
      float dmg = -BaseDmg;

      target->changeHealth(dmg);

      mAttackCountdown = AttackDelay;
      mCanAttack = false;
   }
}
