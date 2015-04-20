#include <iostream>
#include <cmath>
#include <vector>

#include "App/Utility.hpp"
#include "World/World.hpp"
#include "Entity/Enemy.hpp"

Enemy::Enemy(Level* level
            , const sf::Texture& texture
            , const sf::Font& font
            , sf::Vector2f startPos
            , EntityStats stats
            , const Params& params
//            , State<Enemy>* globalState
//            , State<Enemy>* initState
//            , StateContainer& states
//            , Enemy::StateType currentStateType
            , float scale)
: MovingEntity(MovingEntity::EntityType::Enemy
              , level
              , texture
              , font
              , startPos
              , stats
              , params
              , scale)
//, MeleeCombatant(dynamic_cast<MovingEntity*>(this)
//                  , sf::seconds(stats.attackDelay))
, mSightRange(params.EnemySightRange)
, mAngleOfVision(params.EnemyAngleOfVision)
//, mPanicDistance(params.EnemyPanicDistance)
//, mStates(states)
//, mStateMachine(this
//               , globalState
//               , initState
//               , currentStateType)
{
    mText.setPosition(-10.f, -40.f);
}

void Enemy::updateCurrent(sf::Time dt)
{
    mStateMachine.update();

    MovingEntity::updateCurrent(dt);

    mCurrentBlock->deleteEntity(this);
    mCurrentBlock = mLevel->insertEntityIntoLevel(this);

//    MeleeCombatant::update(dt);

//    if(mCurrentBlock->getType() == LevelBlock::Type::ExitBlock)
//    {
//        mToRemove = true;
//        mMovingTarget = nullptr;
//        mCurrentBlock->deleteEntity(this);
//
////        mLevel->incEnemyHerded();
//    }
}

void Enemy::drawCurrent(sf::RenderTarget& target
                        , sf::RenderStates states) const
{
    target.draw(mSprite, states);
    target.draw(mText);
}

//void Enemy::addPursuer(MovingEntity* pursuer)
//{
//  mPursuers.push_back(pursuer);
//}
//
//void Enemy::deletePursuer(MovingEntity* pursuer)
//{
//    auto it = find (mPursuers.begin(), mPursuers.end(), pursuer);
//
//    assert(it != mPursuers.end());
//
//    mPursuers.erase(it);
//}
//
//void Enemy::clearPursuers()
//{
//  mPursuers.clear();
//}

//void Enemy::changeState(Enemy::States newState)
//{
//    mStateMachine.changeState(mStates.at(newState).get());
//}
