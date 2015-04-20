#include <cassert>
#include <iostream>

#include "App/Utility.hpp"
#include "Entity/State/AdventurerStates.hpp"
#include "Entity/SteeringBehaviour.hpp"
//#include <MovingTarget.hpp"
//#include "incl/World.hpp"

using namespace CharacterStates;

void LookOut::enter(Character* host)
{
    assert(host);
}

void LookOut::execute(Character* host)
{
   // If host is currently relaxed
   if(host->getCurrentState() == Character::StateType::Relax)
   {
      // get all close enemies
      std::vector<MovingEntity*> enemies = host->getNeighbours(host->getAgroDistance()
                                                               , MovingEntity::EntityType::Enemy);

      sf::Vector2f hostPos = host->getWorldPosition();

      MovingEntity* closestEnemy = nullptr;
      float closestDist = 9999.f;

      // Loop thorugh enemies to find closest
      for(MovingEntity* e : enemies)
      {
         sf::Vector2f enemyPos = e->targetPosition();
         sf::Vector2f vecToEnemy = hostPos - enemyPos;

         float mag = magVec(vecToEnemy);

         if(mag < closestDist)
         {
            closestEnemy = e;
            closestDist = mag;
         }
      }

      // If enemy target exists
      if(closestEnemy)
      {
         // Change to attack state and set target
         host->changeState(Character::StateType::Attack);
         host->setCurrentTarget(closestEnemy);
      }
   }

}

void LookOut::exit(Character* host)
{

}

//void Evade::enter(Character* host)
//{
//    assert(host);
//
//    std::vector<SteeringBehaviour::Behaviour> behaviours;
//
//    if(!host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Evade))
//        behaviours.push_back(SteeringBehaviour::Behaviour::Evade);
//
//    behaviours.push_back(SteeringBehaviour::Behaviour::Flock);
//
//    host->setSteeringTypes(behaviours);
//    host->setText("!!!!");
//
//    host->setMaxSpeed(host->getMaxRunSpeed());
//}
//
//void Evade::execute(Character* host)
//{
//    if(host->getCurrentTarget())
//    {
//        sf::Vector2f dogPos = host->getCurrentTarget()->targetPosition();
//        sf::Vector2f vecToCharacter = host->getWorldPosition() - dogPos;
//
//        float mag = magVec(vecToCharacter);
//
//        if(mag > host->getPanicDistance())
//        {
//            host->changeState(Character::StateType::Relax);
//        }
//    }
//}
//
//void Evade::exit(Character* host)
//{
////
//}

void Relax::enter(Character* host)
{
    assert(host);

    if(!host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Wander))
    {
        std::vector<SteeringBehaviour::Behaviour> behaviours;
        behaviours.push_back(SteeringBehaviour::Behaviour::Wander);
        host->setSteeringTypes(behaviours);
    }

    host->setText("");
    host->setMaxSpeed(host->getMaxWalkSpeed());
}

void Relax::execute(Character* host)
{

}

void Relax::exit(Character* host)
{

}

void Attack::enter(Character* host)
{
    assert(host);

    if(!host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Arrive))
    {
        std::vector<SteeringBehaviour::Behaviour> behaviours;
        behaviours.push_back(SteeringBehaviour::Behaviour::Arrive);
        host->setSteeringTypes(behaviours);
    }

    host->setText("");
    host->setMaxSpeed(host->getMaxRunSpeed());
}

void Attack::execute(Character* host)
{
   MovingEntity* curTarg = host->getCurrentTarget();

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
      host->changeState(Character::StateType::Relax);
   }
}

void Attack::exit(Character* host)
{

}

//void Follow::enter(Character* host)
//{
//    assert(host);
//
////    if(!host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Wander))
////    {
////        std::vector<SteeringBehaviour::Behaviour> behaviours;
////        behaviours.push_back(SteeringBehaviour::Behaviour::Wander);
////
////        if(isFlocking)
////            behaviours.push_back(SteeringBehaviour::Behaviour::Flock);
////
////        host->setSteeringTypes(behaviours);
////    }
////
//////    host->setText("Baaah");
////    host->setMaxSpeed(host->getMaxWalkSpeed());
//}
//
//void Follow::execute(Character* host)
//{
//
//}
//
//void Follow::exit(Character* host)
//{
//
//}

//void Exit::enter(Character* host)
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
//void Exit::execute(Character* host)
//{
//
//}
//
//void Exit::exit(Character* host)
//{
//
//}

