<<<<<<< HEAD
#include "Entity/Enemy.hpp"

Enemy::Enemy(Enemy::Type enemyType
            , const sf::Texture& texture
            , const sf::Font& font
            , sf::Vector2f startPos
            , float scale
            , const EntityStats& stats
            , const Params& params
            , Level* level
            , State<Enemy>* globalState
            , State<Enemy>* initState
            , StateContainer& states
            , Enemy::StateType currentStateType)
: Entity(texture
         , font
         , startPos
         , scale
         , stats
         , params
         , Entity::EntityType::Enemy
         , level)
, Intelligent(stats)
, Killable(stats.health)
, MeleeFighter(sf::seconds(stats.attackDelay)
                , stats.meleeDamage
                , stats.attackDist)
, mEnemyType(enemyType)
=======
#include <iostream>
#include <cmath>
#include <vector>

#include "App/Utility.hpp"
#include "World/World.hpp"
#include "Entity/Enemy.hpp"

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
: Entity(//world
         level
         , texture
         , font
         , startPos
         , stats
         , params
         , Entity::Type::Enemy
         , params.EnemyPanicDistance
         , scale)

, mSightRange(params.EnemySightRange)
, mAngleOfVision(params.EnemyAngleOfVision)
//, mPanicDistance(params.EnemyPanicDistance)
>>>>>>> working
, mStates(states)
, mStateMachine(this
               , globalState
               , initState
               , currentStateType)
{
<<<<<<< HEAD
//    mText.setPosition(-10.f, -40.f);
};

void Enemy::updateCurrent(sf::Time dt)
{
   mStateMachine.update();
   Entity::updateCurrent(dt);
=======
    mText.setPosition(-10.f, -40.f);

//    mCurrentBlock = mLevel->insertEntityIntoLevel(this);
}

void Enemy::updateCurrent(sf::Time dt)
{
    mStateMachine.update();

    Entity::updateCurrent(dt);

    mCurrentBlock->deleteEntity(this);
    mCurrentBlock = mLevel->insertEntityIntoLevel(this);

//    if(mCurrentBlock->getType() == LevelBlock::Type::ExitBlock)
//    {
//        mToRemove = true;
////        mMovingTarget = nullptr;
//        mCurrentBlock->deleteEntity(this);
//
////        mLevel->incEnemyHerded();
//    }
>>>>>>> working
}

void Enemy::drawCurrent(sf::RenderTarget& target
                            , sf::RenderStates states) const
{
   Entity::drawCurrent(target, states);
}
