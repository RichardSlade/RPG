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
, mStates(states)
, mStateMachine(this
               , globalState
               , initState
               , currentStateType)
{
//    mText.setPosition(-10.f, -40.f);
};

void Enemy::updateCurrent(sf::Time dt)
{
   mStateMachine.update();
   Entity::updateCurrent(dt);
}

void Enemy::drawCurrent(sf::RenderTarget& target
                            , sf::RenderStates states) const
{
   Entity::drawCurrent(target, states);
}
