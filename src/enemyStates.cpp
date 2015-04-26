#include <cassert>
#include <iostream>

#include "App/Utility.hpp"
#include "World/World.hpp"
//#include "Entity/Adventurer.hpp"
#include "Entity/State/EnemyStates.hpp"
#include "Entity/SteeringBehaviour.hpp"
#include "Entity/Entity.hpp"

using namespace EnemyStates;

void LookOut::enter(Enemy* host)
{
    assert(host);
}

void LookOut::execute(Enemy* host)
{
	if(host->getCurrentStateType() == Enemy::States::Relax)
	{
      std::vector<Entity*> chars;

      bool isHealthy = host->getHealthPercentage() > 50.f ? true : false;

      if(isHealthy) // Get all chars in agro distance
         chars = host->getNeighbours(host->AgroDistance
                                    , Entity::Type::Adventurer);
      else // Get all chars in panic distance
         chars = host->getNeighbours(host->PanicDistance
                                    , Entity::Type::Adventurer);

      sf::Vector2f hostPos = host->getWorldPosition();

      Entity* closestChar = nullptr;
      float closestDist = 9999.f;

      // Loop thorugh enemies to find closest
      for(Entity* e : chars)
      {
         sf::Vector2f vecToEnemy = hostPos - e->getWorldPosition();
         float mag = magVec(vecToEnemy);

         if(mag < closestDist)
         {
            closestChar = e;
            closestDist = mag;
         }
      }

      // If host target exists set it as target
      // and change state accordingly to health
      if(closestChar)
      {
         host->setCurrentTarget(closestChar);

//         std::cout << "enemyStates.cpp: LookOut(): closest char pos = " << closestChar->getWorldPosition().x << ", " << closestChar->getWorldPosition().y << std::endl;

         if(isHealthy)
            host->changeState(Enemy::States::Attack);
         else
            host->changeState(Enemy::States::Evade);
      }
	}
}

void LookOut::exit(Enemy* host)
{

}

void Evade::enter(Enemy* host)
{
    assert(host);

    std::vector<SteeringBehaviour::Behaviour> behaviours;

    if(!host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Evade))
        behaviours.push_back(SteeringBehaviour::Behaviour::Evade);

//    behaviours.push_back(SteeringBehaviour::Behaviour::Flock);

    host->setSteeringTypes(behaviours);
    host->setText("!!!!");

    host->setMaxSpeed(host->getMaxRunSpeed());
}

void Evade::execute(Enemy* host)
{
   const Entity* pursuer = host->getCurrentTarget();
//   Adventurer* pursuer = nullptr;

   if(pursuer)
   {
      sf::Vector2f vecToHost = host->getWorldPosition() - pursuer->getWorldPosition();
      float mag = magVec(vecToHost);

      if(mag > host->PanicDistance)
      {
         host->changeState(Enemy::States::Relax);
      }
   }
}

void Evade::exit(Enemy* host)
{

}

void Relax::enter(Enemy* host)
{
    assert(host);

    if(!host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Wander))
    {
        std::vector<SteeringBehaviour::Behaviour> behaviours;
        behaviours.push_back(SteeringBehaviour::Behaviour::Wander);

        host->setSteeringTypes(behaviours);
    }

    host->setText("Grr");
    host->setMaxSpeed(host->getMaxWalkSpeed());
}

void Relax::execute(Enemy* host)
{

}

void Relax::exit(Enemy* host)
{

}

void Attack::enter(Enemy* host)
{
    assert(host);

    if(!host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Arrive))
    {
        std::vector<SteeringBehaviour::Behaviour> behaviours;
        behaviours.push_back(SteeringBehaviour::Behaviour::Arrive);
        host->setSteeringTypes(behaviours);
    }

    host->setText("Arrggh!");
    host->setMaxSpeed(host->getMaxRunSpeed());
}

void Attack::execute(Enemy* host)
{
   const Entity* curTarg = host->getCurrentTarget();
//   Adventurer* curTarg = nullptr;

   if(curTarg
      && !curTarg->isDead()) // If there is current target
   {
      sf::Vector2f vecToTarget = curTarg->getWorldPosition() - host->getWorldPosition();
      float mag = magVec(vecToTarget);

      // If target close enough to attack
      if(mag < host->AttackDistance)
      {
         // If not at stand still change steering behaviour
         if(!host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Rest))
         {
            std::vector<SteeringBehaviour::Behaviour> behaviours;
            behaviours.push_back(SteeringBehaviour::Behaviour::Rest);
            behaviours.push_back(SteeringBehaviour::Behaviour::Face);
            host->setSteeringTypes(behaviours);
         }

         // Attack current target (overloaded function)
         host->meleeAttack(const_cast<Entity*>(curTarg));

         host->setText("Yarrr!");
      }
      else // Approach target
      {
         if(!host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Arrive))
         {
            std::vector<SteeringBehaviour::Behaviour> behaviours;
            behaviours.push_back(SteeringBehaviour::Behaviour::Arrive);
            host->setSteeringTypes(behaviours);
         }
      }
   }
   else // If no target
   {
      host->changeState(Enemy::States::Relax);
   }
}

void Attack::exit(Enemy* host)
{

}

//void Exit::enter(Enemy* host)
//{
//    assert(host);
//
//    if(!host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Arrive))
//    {
//        std::vector<SteeringBehaviour::Behaviour> behaviours;
//        behaviours.push_back(SteeringBehaviour::Behaviour::Arrive);
//
//        host->setSteeringTypes(behaviours);
//    }
//
//    host->setText("Maaah");
//    host->setMaxSpeed(host->getMaxRunSpeed());
//}
//
//void Exit::execute(Enemy* host)
//{
//
//}
//
//void Exit::exit(Enemy* host)
//{
//
//}
