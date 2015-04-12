#include <iostream>
#include <cmath>
#include <vector>

#include "incl/Enemy.hpp"
#include "incl/Utility.hpp"
#include "incl/World.hpp"

Enemy::Enemy(//World* world
			Level* level
		  , const sf::Texture& texture
		  , const sf::Font& font
		  , sf::Vector2f startPos
		  , EntityStats stats
		  , const Params& params
		  , State<Enemy>* globalState
		  , State<Enemy>* initState
		  , StateContainer& states
		  , float scale)
: MovingEntity(//world
					level
               , texture
               , font
               , startPos
               , stats
               , params
               , MovingEntity::EntityType::Enemy
               , params.EnemyPanicDistance
               , scale)

, mSightRange(params.EnemySightRange)
, mAngleOfVision(params.EnemyAngleOfVision)
//, mPanicDistance(params.EnemyPanicDistance)
, mStates(states)
, mStateMachine(this, globalState, initState)
{
    mText.setPosition(-10.f, -40.f);

    mCurrentBlock = mLevel->insertEntityIntoLevel(this);
}

void Enemy::updateCurrent(sf::Time dt)
{
    mStateMachine.update();

    MovingEntity::updateCurrent(dt);

    mCurrentBlock->deleteEntity(this);
    mCurrentBlock = mLevel->insertEntityIntoLevel(this);

    if(mCurrentBlock->getType() == LevelBlock::Type::ExitBlock)
    {
        mToRemove = true;
        mMovingTarget = nullptr;
        mCurrentBlock->deleteEntity(this);

//        mLevel->incEnemyHerded();
    }
}

void Enemy::drawCurrent(sf::RenderTarget& target
                        , sf::RenderStates states) const
{
    target.draw(mSprite, states);
    target.draw(mText);
}

//void Enemy::changeState(Enemy::States newState)
//{
//    mStateMachine.changeState(mStates.at(newState).get());
//}
