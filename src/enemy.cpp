#include <iostream>
#include <cmath>
#include <vector>

#include "App/Utility.hpp"
#include "World/World.hpp"
#include "Entity/Enemy.hpp"

Enemy::Enemy(QuadTree* quadTree
, const sf::Texture& texture
, const sf::Font& font
, sf::Vector2f pos
, EntityStats stats
, const Params& params
, State<Enemy>* globalState
, State<Enemy>* initState
, StateContainer& states
, unsigned int currentState
, b2Body* body
, float scale)
: Entity(quadTree
         , texture
         , font
         , pos
         , stats
         , params
         , Entity::Type::Enemy
         , body
         , scale)
, mSightRange(params.EnemySightRange)
, mAngleOfVision(params.EnemyAngleOfVision)
//, mPanicDistance(params.EnemyPanicDistance)
, mStates(states)
, mStateMachine(this, globalState, initState, currentState)
{
//    mText.setPosition(-10.f, -40.f);

//    mCurrentBlock = mLevel->insertEntityIntoLevel(this);
}

void Enemy::updateCurrent(sf::Time dt)
{
   mStateMachine.update();

   Entity::updatePhysicsBody(dt);
   Entity::updateCurrent(dt);
}

void Enemy::drawCurrent(sf::RenderTarget& target
                        , sf::RenderStates states) const
{
   Entity::drawCurrent(target, states);
}

//void Enemy::changeState(Enemy::States newState)
//{
//    mStateMachine.changeState(mStates.at(newState).get());
//}
