#include <cassert>
#include <iostream>

#include "incl/CharacterStates.hpp"
#include "incl/SteeringBehaviour.hpp"
#include "incl/Utility.hpp"
#include "incl/MovingTarget.hpp"
//#include "incl/World.hpp"

void LookOutForEnemy::enter(Character* character)
{
    assert(character);
}

void LookOutForEnemy::execute(Character* character)
{
    const MovingTarget* enemy = character->getMovingTarget();

    if(enemy)
    {
        sf::Vector2f characterPos = character->getWorldPosition();

//        std::vector<LevelBlock*> levelExit = character->getLevelExit();

//        bool closeToExit = false;
        float expandedRadius = 100 + character->getRadius();

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
//            character->changeState(Character::States::Exit);
//            character->setTargetPos(targPos);
//            return;
//        }

        sf::Vector2f enemyPos = enemy->targetPosition();
        sf::Vector2f vecToEnemy = characterPos - enemyPos;

        float mag = magVec(vecToEnemy);

//        if(mag <= character->mPanicDistance * 2.f)
//            character->changeState(Character::States::Evade);
    }
}

void LookOutForEnemy::exit(Character* character)
{

}

void EvadeEnemy::enter(Character* character)
{
    assert(character);

    std::vector<SteeringBehaviour::Behaviour> behaviours;

    if(!character->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Evade))
        behaviours.push_back(SteeringBehaviour::Behaviour::Evade);

    behaviours.push_back(SteeringBehaviour::Behaviour::Flock);

    character->setSteeringTypes(behaviours);
    character->setText("!!!!");

    character->setMaxSpeed(character->getMaxRunSpeed());
}

void EvadeEnemy::execute(Character* character)
{
    if(character->getMovingTarget())
    {
        sf::Vector2f dogPos = character->getMovingTarget()->targetPosition();
        sf::Vector2f vecToCharacter = character->getWorldPosition() - dogPos;

        float mag = magVec(vecToCharacter);

        if(mag > character->mPanicDistance)
        {
            character->changeState(Character::States::Relax);
        }
    }
}

void EvadeEnemy::exit(Character* character)
{

}

void RelaxChar::enter(Character* character)
{
    assert(character);

    bool isFlocking = false;

    if(character->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Flock))
        isFlocking = true;

    if(!character->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Wander))
    {
        std::vector<SteeringBehaviour::Behaviour> behaviours;
        behaviours.push_back(SteeringBehaviour::Behaviour::Wander);

        if(isFlocking)
            behaviours.push_back(SteeringBehaviour::Behaviour::Flock);

        character->setSteeringTypes(behaviours);
    }

    character->setText("Baaah");
    character->setMaxSpeed(character->getMaxWalkSpeed());
}

void RelaxChar::execute(Character* character)
{

}

void RelaxChar::exit(Character* character)
{

}

void FollowCharacter::enter(Character* character)
{
    assert(character);

    if(!character->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Wander))
    {
        std::vector<SteeringBehaviour::Behaviour> behaviours;
        behaviours.push_back(SteeringBehaviour::Behaviour::Wander);

        if(isFlocking)
            behaviours.push_back(SteeringBehaviour::Behaviour::Flock);

        character->setSteeringTypes(behaviours);
    }

//    character->setText("Baaah");
    character->setMaxSpeed(character->getMaxWalkSpeed());
}

void FollowCharacter::execute(Character* character)
{

}

void FollowCharacter::exit(Character* character)
{

}

//void Exit::enter(Character* character)
//{
//    assert(character);
//
//    if(!character->checkSteeringBehaviour(SteeringBehaviour::Behaviour::Arrive))
//    {
//        std::vector<SteeringBehaviour::Behaviour> behaviours;
//        behaviours.push_back(SteeringBehaviour::Behaviour::Arrive);
//
//        character->setSteeringTypes(behaviours);
//    }
//
//    character->setText("Maaah");
//    character->setMaxSpeed(character->getMaxRunSpeed());
//}
//
//void Exit::execute(Character* character)
//{
//
//}
//
//void Exit::exit(Character* character)
//{
//
//}

