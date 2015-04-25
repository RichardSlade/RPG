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
	if(host->getCurrentStateType() == Enemy::StateType::Relax)
	{
      std::vector<Entity*> chars;

      bool isHealthy = host->getHealthPercentage() > 50.f ? true : false;

      if(isHealthy) // Get all chars in agro distance
         chars = host->getNeighbours(host->agroDistance
                                    , Entity::EntityType::Adventurer);
      else // Get all chars in panic distance
         chars = host->getNeighbours(host->panicDistance
                                    , Entity::EntityType::Adventurer);

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
//         host->setCurrentTarget(closestChar);

         if(isHealthy)
            host->changeState(Enemy::StateType::Attack);
         else
            host->changeState(Enemy::StateType::Evade);
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
//   Entity* pursuer = host->getCurrentTarget();
   Adventurer* pursuer = nullptr;

   if(pursuer)
   {
      sf::Vector2f vecToHost = host->getWorldPosition() - pursuer->getWorldPosition();
      float mag = magVec(vecToHost);

      if(mag > host->panicDistance)
      {
         host->changeState(Enemy::StateType::Relax);
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
//   Adventurer* curTarg = host->getCurrentTarget();
   Adventurer* curTarg = nullptr;

   if(curTarg) // If there is current target
   {
      sf::Vector2f vecToTarget = curTarg->getWorldPosition() - host->getWorldPosition();
      float mag = magVec(vecToTarget);

      // If target close enough to attack
      if(mag < host->attackDistance)
      {
         // If not at stand still change steering behaviour
         if(!host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Rest))
         {
            std::vector<SteeringBehaviour::Behaviour> behaviours;
            behaviours.push_back(SteeringBehaviour::Behaviour::Rest);
            host->setSteeringTypes(behaviours);
         }

         // Attack current target (overloaded function)
         host->meleeAttack(curTarg);
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
      host->changeState(Enemy::StateType::Relax);
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
