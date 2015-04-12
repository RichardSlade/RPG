#include <cassert>
#include <iostream>

#include "incl/EnemyStates.hpp"
#include "incl/SteeringBehaviour.hpp"
#include "incl/Utility.hpp"
#include "incl/MovingTarget.hpp"
#include "incl/World.hpp"

void LookOutForCharacter::enter(Enemy* enemy)
{
    assert(enemy);
}

void LookOutForCharacter::execute(Enemy* enemy)
{
    const MovingTarget* dog = enemy->getMovingTarget();

    if(dog
       && !enemy->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Arrive))
    {
        sf::Vector2f enemyPos = enemy->getWorldPosition();

        std::vector<LevelBlock*> levelExit = enemy->getLevelExit();

        bool closeToExit = false;
        float expandedRadius = 100 + enemy->getRadius();

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
//            enemy->changeState(Enemy::States::Exit);
//            enemy->setTargetPos(targPos);
//            return;
//        }

        sf::Vector2f dogPos = dog->targetPosition();
        sf::Vector2f vecToCharacter = enemyPos - dogPos;

        float magCharacter = magVec(vecToCharacter);

        if(magCharacter <= enemy->mPanicDistance * 2.f)
            enemy->changeState(Enemy::States::Evade);
    }
}

void LookOutForCharacter::exit(Enemy* enemy)
{

}

void EvadeCharacter::enter(Enemy* enemy)
{
    assert(enemy);

    std::vector<SteeringBehaviour::Behaviour> behaviours;

    if(!enemy->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Evade))
        behaviours.push_back(SteeringBehaviour::Behaviour::Evade);

    behaviours.push_back(SteeringBehaviour::Behaviour::Flock);

    enemy->setSteeringTypes(behaviours);
    enemy->setText("!!!!");

    enemy->setMaxSpeed(enemy->getMaxRunSpeed());
}

void EvadeCharacter::execute(Enemy* enemy)
{
    if(enemy->getMovingTarget())
    {
        sf::Vector2f dogPos = enemy->getMovingTarget()->targetPosition();
        sf::Vector2f vecToCharacter = enemy->getWorldPosition() - dogPos;

        float mag = magVec(vecToCharacter);

        if(mag > enemy->mPanicDistance)
        {
            enemy->changeState(Enemy::States::Relax);
        }
    }
}

void EvadeCharacter::exit(Enemy* enemy)
{

}

void RelaxEnemy::enter(Enemy* enemy)
{
    assert(enemy);

    bool isFlocking = false;

    if(enemy->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Flock))
        isFlocking = true;

    if(!enemy->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Wander))
    {
        std::vector<SteeringBehaviour::Behaviour> behaviours;
        behaviours.push_back(SteeringBehaviour::Behaviour::Wander);

        if(isFlocking)
            behaviours.push_back(SteeringBehaviour::Behaviour::Flock);

        enemy->setSteeringTypes(behaviours);
    }

    enemy->setText("Baaah");
    enemy->setMaxSpeed(enemy->getMaxWalkSpeed());
}

void RelaxEnemy::execute(Enemy* enemy)
{

}

void RelaxEnemy::exit(Enemy* enemy)
{

}

//void Exit::enter(Enemy* enemy)
//{
//    assert(enemy);
//
//    if(!enemy->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Arrive))
//    {
//        std::vector<SteeringBehaviour::Behaviour> behaviours;
//        behaviours.push_back(SteeringBehaviour::Behaviour::Arrive);
//
//        enemy->setSteeringTypes(behaviours);
//    }
//
//    enemy->setText("Maaah");
//    enemy->setMaxSpeed(enemy->getMaxRunSpeed());
//}
//
//void Exit::execute(Enemy* enemy)
//{
//
//}
//
//void Exit::exit(Enemy* enemy)
//{
//
//}
