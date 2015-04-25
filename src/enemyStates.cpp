#include <cassert>
#include <iostream>

#include "App/Utility.hpp"
#include "World/World.hpp"
#include "Entity/State/EnemyStates.hpp"
#include "Entity/SteeringBehaviour.hpp"
#include "Entity/Entity.hpp"
//#include "Entity/MovingTarget.hpp"

using namespace EnemyStates;

void LookOut::enter(Enemy* host)
{
    assert(host);
}

void LookOut::execute(Enemy* host)
{
    const Entity* adventurer = host->getCurrentTarget();

    if(adventurer
       && !host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Arrive))
    {
        sf::Vector2f enemyPos = host->getWorldPosition();

        std::vector<LevelBlock*> levelExit = host->getLevelExit();

        bool closeToExit = false;
        float expandedRadius = 100 + host->getRadius();

        sf::Vector2f targPos;

        for(LevelBlock* lvlBlck : levelExit)
        {
            sf::Vector2f blckPos = lvlBlck->getMiddle();

            sf::Vector2f toBlck = blckPos - enemyPos;
            float magExit = magVec(toBlck);

            if(magExit < expandedRadius)
            {
                closeToExit = true;
                targPos = blckPos;
                break;
            }
        }

//        if(closeToExit)
//        {
//            host->changeState(Enemy::States::Exit);
//            host->setTargetPos(targPos);
//            return;
//        }

        sf::Vector2f adventurerPos = adventurer->getWorldPosition();
        sf::Vector2f vecTo = enemyPos - adventurerPos;

        float mag = magVec(vecTo);

        if(mag <= host->mPanicDistance * 2.f)
            host->changeState(Enemy::States::Evade);
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
    if(host->getCurrentTarget())
    {
        sf::Vector2f adventurerPos = host->getCurrentTarget()->getWorldPosition();
        sf::Vector2f vecTo = host->getWorldPosition() - adventurerPos;

        float mag = magVec(vecTo);

        if(mag > host->mPanicDistance)
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

    bool isFlocking = false;

    if(host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Flock))
        isFlocking = true;

    if(!host->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Wander))
    {
        std::vector<SteeringBehaviour::Behaviour> behaviours;
        behaviours.push_back(SteeringBehaviour::Behaviour::Wander);

//        if(isFlocking)
//            behaviours.push_back(SteeringBehaviour::Behaviour::Flock);

        host->setSteeringTypes(behaviours);
    }

    host->setText("Grrr");
    host->setMaxSpeed(host->getMaxWalkSpeed());
}

void Relax::execute(Enemy* host)
{

}

void Relax::exit(Enemy* host)
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
