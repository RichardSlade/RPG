#include <cassert>
#include <iostream>

#include "App/Utility.hpp"
#include "World/World.hpp"
#include "Entity/GoblinStates.hpp"
#include "Entity/SteeringBehaviour.hpp"
#include "Entity/Entity.hpp"

using namespace EnemyStates;

void LookOut::enter(Goblin* host)
{
    assert(host);
}

void LookOut::execute(Goblin* host)
{
	if(host->getCurrentState() == Goblin::StateType::Relax)
	{
      std::vector<Entity*> chars;

      bool isHealthy = host->getHealthPercentage() > 50.f ? true : false;

      if(isHealthy) // Get all chars in agro distance
         chars = host->getNeighbours(host->getAgroDistance()
                                    , Entity::EntityType::Character);
      else // Get all chars in panic distance
         chars = host->getNeighbours(host->getPanicDistance()
                                    , Entity::EntityType::Character);

      sf::Vector2f hostPos = host->getWorldPosition();

      Entity* closestChar = nullptr;
      float closestDist = 9999.f;

      // Loop thorugh enemies to find closest
      for(Entity* e : chars)
      {
         sf::Vector2f vecToEnemy = hostPos - e->targetPosition();
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

         if(isHealthy)
            host->changeState(Goblin::StateType::Attack);
         else
            host->changeState(Goblin::StateType::Evade);
      }
	}
}

void LookOut::exit(Goblin* host)
{

}

void Evade::enter(Goblin* host)
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

void Evade::execute(Goblin* host)
{
   Entity* pursuer = host->getCurrentTarget();

   sf::Vector2f vecToHost = host->getWorldPosition() - pursuer->getWorldPosition();
   float mag = magVec(vecToHost);

   if(mag > host->getPanicDistance())
   {
      host->changeState(Goblin::StateType::Relax);
   }
}

void Evade::exit(Goblin* host)
{

}

void Relax::enter(Goblin* host)
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

void Relax::execute(Goblin* host)
{

}

void Relax::exit(Goblin* host)
{

}

void Attack::enter(Goblin* host)
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

void Attack::execute(Goblin* host)
{
   Entity* curTarg = host->getCurrentTarget();

   if(curTarg) // If there is current target
   {
      sf::Vector2f vecToTarget = curTarg->getWorldPosition() - host->getWorldPosition();
      float mag = magVec(vecToTarget);

      // If target close enough to attack
      if(mag < host->getAttackDistance())
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
      host->changeState(Goblin::StateType::Relax);
   }
}

void Attack::exit(Goblin* host)
{

}

//void Exit::enter(Goblin* host)
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
//void Exit::execute(Goblin* host)
//{
//
//}
//
//void Exit::exit(Goblin* host)
//{
//
//}
