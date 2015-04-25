#include <cassert>
<<<<<<< HEAD
//#include <iostream>

#include "App/Utility.hpp"
#include "Entity/Enemy.hpp"
#include "Entity/State/AdventurerStates.hpp"
#include "Entity/SteeringBehaviour.hpp"
//#include "Entity/Ag.hpp"
//#include <MovingTarget.hpp"
//#include "incl/World.hpp"
=======
#include <iostream>

#include "App/Utility.hpp"
#include "Entity/State/AdventurerStates.hpp"
#include "Entity/SteeringBehaviour.hpp"
#include "Entity/Entity.hpp"
//#include "World.hpp"
>>>>>>> working

using namespace AdventurerStates;

void LookOut::enter(Adventurer* host)
{
    assert(host);
}

void LookOut::execute(Adventurer* host)
{
<<<<<<< HEAD
   // If host is currently relaxed
   if(host->getCurrentState() == Adventurer::StateType::Relax)
   {
      // get all close enemies
      std::vector<Entity*> enemies = host->getNeighbours(host->agroDistance
                                                               , Entity::EntityType::Enemy);

      sf::Vector2f hostPos = host->getWorldPosition();

      Entity* closestEnemy = nullptr;
      float closestDist = 9999.f;

      // Loop thorugh enemies to find closest
      for(Entity* e : enemies)
      {
         sf::Vector2f vecToEnemy = hostPos - e->getWorldPosition();

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
//         host->changeState(Adventurer::StateType::Attack);
//         host->setCurrentTarget(closestEnemy);
      }
   }

}

void LookOut::exit(Adventurer* host)
{

}

//void Evade::enter(Adventurer* host)
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
//void Evade::execute(Adventurer* host)
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
//            host->changeState(Adventurer::StateType::Relax);
//        }
//    }
//}
//
//void Evade::exit(Adventurer* host)
//{
////
//}

void Relax::enter(Adventurer* host)
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

void Relax::execute(Adventurer* host)
{

}

void Relax::exit(Adventurer* host)
=======
    const Entity* enemy = host->getCurrentTarget();

    if(enemy)
    {
        sf::Vector2f characterPos = host->getWorldPosition();

//        std::vector<LevelBlock*> levelExit = host->getLevelExit();

//        bool closeToExit = false;
        float expandedRadius = 100 + host->getRadius();

        sf::Vector2f targPos;

//        for(LevelBlock* lvlBlck : levelExit)
//        {
//            sf::Vector2f blckPos = lvlBlck->getMiddle();
//
//            sf::Vector2f toBlck = blckPos - characterPos;
//            float magExit = magVec(toBlck);
//
//            if(magExit < expandedRadius)
//            {
//                closeToExit = true;
//                targPos = blckPos;
//                break;
//            }
//        }

//        if(closeToExit)
//        {
//            host->changeState(Adventurer::States::Exit);
//            host->setTargetPos(targPos);
//            return;
//        }

        sf::Vector2f enemyPos = enemy->getWorldPosition();
        sf::Vector2f vecToEnemy = characterPos - enemyPos;

        float mag = magVec(vecToEnemy);

//        if(mag <= host->mPanicDistance * 2.f)
//            host->changeState(Adventurer::States::Evade);
    }
}

void LookOut::exit(Adventurer* host)
{

}

void Evade::enter(Adventurer* host)
{
    assert(host);

    std::vector<SteeringBehaviour::Behaviour> behaviours;

    if(!host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Evade))
        behaviours.push_back(SteeringBehaviour::Behaviour::Evade);

    behaviours.push_back(SteeringBehaviour::Behaviour::Flock);

    host->setSteeringTypes(behaviours);
    host->setText("!!!!");

    host->setMaxSpeed(host->getMaxRunSpeed());
}

void Evade::execute(Adventurer* host)
{
    if(host->getCurrentTarget())
    {
        sf::Vector2f dogPos = host->getCurrentTarget()->getWorldPosition();
        sf::Vector2f vecToCharacter = host->getWorldPosition() - dogPos;

        float mag = magVec(vecToCharacter);

        if(mag > host->mPanicDistance)
        {
            host->changeState(Adventurer::States::Relax);
        }
    }
}

void Evade::exit(Adventurer* host)
>>>>>>> working
{

}

<<<<<<< HEAD
void Attack::enter(Adventurer* host)
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

void Attack::execute(Adventurer* host)
{
   Enemy* curTarg = nullptr;
//   Entity* curTarg = host->getCurrentTarget();

   if(curTarg) // If there is current target
   {
      sf::Vector2f vecToTarget = curTarg->getWorldPosition() - host->getWorldPosition();
      float mag = magVec(vecToTarget);

      // If target close enough to attack
      if(mag < host->mAttackDistance)
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
      host->changeState(Adventurer::StateType::Relax);
   }
}

void Attack::exit(Adventurer* host)
=======
void Relax::enter(Adventurer* host)
{
    assert(host);

    bool isFlocking = false;

    if(host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Flock))
        isFlocking = true;

    if(!host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Wander))
    {
        std::vector<SteeringBehaviour::Behaviour> behaviours;
        behaviours.push_back(SteeringBehaviour::Behaviour::Wander);

        if(isFlocking)
            behaviours.push_back(SteeringBehaviour::Behaviour::Flock);

        host->setSteeringTypes(behaviours);
    }

    host->setText("Baaah");
    host->setMaxSpeed(host->getMaxWalkSpeed());
}

void Relax::execute(Adventurer* host)
{

}

void Relax::exit(Adventurer* host)
>>>>>>> working
{

}

//void Follow::enter(Adventurer* host)
//{
//    assert(host);
//
<<<<<<< HEAD
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
//void Follow::execute(Adventurer* host)
=======
//    if(!host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Wander))
//    {
//        std::vector<SteeringBehaviour::Behaviour> behaviours;
//        behaviours.push_back(SteeringBehaviour::Behaviour::Wander);
//
////        if(isFlocking)
//            behaviours.push_back(SteeringBehaviour::Behaviour::Flock);
//
//        host->setSteeringTypes(behaviours);
//    }
//
////    host->setText("Baaah");
//    host->setMaxSpeed(host->getMaxWalkSpeed());
//}
//
//void FollowCharacter::execute(Adventurer* host)
>>>>>>> working
//{
//
//}
//
<<<<<<< HEAD
//void Follow::exit(Adventurer* host)
=======
//void FollowCharacter::exit(Adventurer* host)
>>>>>>> working
//{
//
//}

//void Exit::enter(Adventurer* host)
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
//void Exit::execute(Adventurer* host)
//{
//
//}
//
//void Exit::exit(Adventurer* host)
//{
//
//}

