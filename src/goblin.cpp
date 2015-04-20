#include "Entity/Goblin.hpp"

Goblin::Goblin(Level* level
            , const sf::Texture& texture
            , const sf::Font& font
            , sf::Vector2f startPos
            , EntityStats stats
            , const Params& params
            , State<Enemy>* globalState
            , State<Enemy>* initState
            , StateContainer& states
            , Enemy::StateType currentStateType
            , float scale)
: Enemy(level
        , texture
        , font
        , startPos
        , stats
        , params
        , scale)
, Killable(stats.health)
, Intelligent(stats)
, MeleeCombatant(dynamic_cast<MovingEntity*>(this)
                  , sf::seconds(stats.attackDelay))
, mSightRange(params.EnemySightRange)
, mAngleOfVision(params.EnemyAngleOfVision)
//, mPanicDistance(params.EnemyPanicDistance)
, mStates(states)
, mStateMachine(this
               , globalState
               , initState
               , currentStateType)
{
    mText.setPosition(-10.f, -40.f);
}

void Goblin::update(sf::Time dt)
{
   Enemy::update(dt);
   Melee::update(dt);
}
